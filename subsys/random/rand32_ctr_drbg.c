/*
 * Copyright (c) 2019, NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <init.h>
#include <device.h>
#include <entropy.h>
#include <kernel.h>
#include <string.h>

#if defined(CONFIG_MBEDTLS)
#if !defined(CONFIG_MBEDTLS_CFG_FILE)
#include "mbedtls/config.h"
#else
#include CONFIG_MBEDTLS_CFG_FILE
#endif /* CONFIG_MBEDTLS_CFG_FILE */
#include <mbedtls/ctr_drbg.h>

#elif defined(CONFIG_TINYCRYPT)

#include <tinycrypt/ctr_prng.h>
#include <tinycrypt/aes.h>
#include <tinycrypt/constants.h>

#endif /* CONFIG_MBEDTLS */

static K_SEM_DEFINE(state_sem, 1, 1);

static struct device *entropy_driver;
static const unsigned char drbg_seed[] = CONFIG_CS_CTR_DRBG_PERSONALIZATION;

#if defined(CONFIG_MBEDTLS)

static mbedtls_ctr_drbg_context ctr_ctx;

static int ctr_drbg_entropy_func(void *ctx, unsigned char *buf, size_t len)
{
	return entropy_get_entropy(ctx, (void *)buf, len);
}

#elif defined(CONFIG_TINYCRYPT)

static TCCtrPrng_t ctr_ctx;

#endif /* CONFIG_MBEDTLS */


static int ctr_drbg_initialize(void)
{
	int ret;

	/* Only one entropy device exists, so this is safe even
	 * if the whole operation isn't atomic.
	 */
	entropy_driver = device_get_binding(CONFIG_ENTROPY_NAME);
	if (!entropy_driver) {
		__ASSERT((entropy_driver != NULL),
			"Device driver for %s (CONFIG_ENTROPY_NAME) not found. "
			"Check your build configuration!",
			CONFIG_ENTROPY_NAME);
		return -EINVAL;
	}

#if defined(CONFIG_MBEDTLS)

	mbedtls_ctr_drbg_init(&ctr_ctx);

	ret = mbedtls_ctr_drbg_seed(&ctr_ctx,
				    ctr_drbg_entropy_func,
				    entropy_driver,
				    drbg_seed,
				    sizeof(drbg_seed));

	if (ret != 0) {
		mbedtls_ctr_drbg_free(&ctr_ctx);
		return -EIO;
	}

#elif defined(CONFIG_TINYCRYPT)

	u8_t entropy[TC_AES_KEY_SIZE + TC_AES_BLOCK_SIZE];

	entropy_get_entropy(entropy_driver, (void *)&entropy, sizeof(entropy));

	ret = tc_ctr_prng_init(&ctr_ctx,
			       (uint8_t *)&entropy,
			       sizeof(entropy),
			       (uint8_t *)drbg_seed,
			       sizeof(drbg_seed));

	if (ret == TC_CRYPTO_FAIL) {
		return -EIO;
	}

#endif

	return 0;
}


int sys_csrand_get(void *dst, u32_t outlen)
{
	int ret;
	unsigned int key = irq_lock();

	if (unlikely(!entropy_driver)) {
		ret = ctr_drbg_initialize();
		if (ret != 0) {
			return ret;
		}
	}

#if defined(CONFIG_MBEDTLS)

	ret = mbedtls_ctr_drbg_random(&ctr_ctx, (unsigned char *)dst, outlen);

#elif defined(CONFIG_TINYCRYPT)

	u8_t entropy[TC_AES_KEY_SIZE + TC_AES_BLOCK_SIZE];

	ret = tc_ctr_prng_generate(&ctr_ctx, 0, 0, (uint8_t *)dst, outlen);

	if (ret == TC_CRYPTO_SUCCESS) {
		ret = 0;
	} else if (ret == TC_CTR_PRNG_RESEED_REQ) {

		entropy_get_entropy(entropy_driver,
				    (void *)&entropy, sizeof(entropy));

		ret = tc_ctr_prng_reseed(&ctr_ctx,
					entropy,
					sizeof(entropy),
					drbg_seed,
					sizeof(drbg_seed));

		ret = tc_ctr_prng_generate(&ctr_ctx, 0, 0,
					   (uint8_t *)dst, outlen);

		ret = (ret == TC_CRYPTO_SUCCESS) ? 0 : -EIO;
	} else {
		ret = -EIO;
	}
#endif
	irq_unlock(key);

	return ret;
}
