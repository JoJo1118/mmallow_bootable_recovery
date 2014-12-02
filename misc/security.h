/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _SECURITY_H_
#define _SECURITY_H_

#define BOOT_IMG        "boot.img"
#define RECOVERY_IMG    "recovery.img"
#define BOOTLOADER_IMG  "bootloader.img"
#define ARRAY_SIZE(x)   (int)(sizeof(x)/sizeof(x[0]))

#define NORMALBOOT_NAME_SIZE   16
#define NORMALBOOT_ARGS_SIZE   512
#define NORMALBOOT_MAGIC_SIZE  8
#define NORMALBOOT_MAGIC       "ANDROID!"

#define SECUREBOOT_MAGIC       "AMLSECU!"
#define SECUREBOOT_MAGIC_SIZE  16
#define SECUREBOOT_MAGIC_VESRION 0x0801

#define DEFEND_KEY \
        "/dev/defendkey"
#define SECURE_CHECK \
        "/sys/class/defendkey/defendkey/secure_check"

#ifndef SECURITY_DEBUG
#define secureDbg(fmt ...)
#else
#define secureDbg(fmt ...) printf(fmt)
#endif

typedef enum SecureCheck {
    FAIL,
    ENCRYPT,
    UNENCRYPT,
    TYPE_MAX,
} T_SecureCheck;

static const char *s_pStatus[TYPE_MAX] = {
    "fail",
    "encrypt",
    "raw",
};

typedef struct NormalBootImgHdr {
    unsigned char magic[NORMALBOOT_MAGIC_SIZE];
    unsigned kernel_size;
    unsigned kernel_addr;
    unsigned ramdisk_size;
    unsigned ramdisk_addr;
    unsigned second_size;
    unsigned second_addr;
    unsigned tags_addr; // physical addr for kernel tags
    unsigned page_size; // flash page size we assume
    unsigned unused[2];
    unsigned char name[NORMALBOOT_NAME_SIZE];
    unsigned char cmdline[NORMALBOOT_ARGS_SIZE];
    unsigned id[8];
} T_NormalBootImgHdr;

typedef struct EncryptBootImgInfo {
    // magic to identify whether it is a encrypted boot image
    unsigned char magic[SECUREBOOT_MAGIC_SIZE];

    // version for this header struct
    unsigned int  version;

    // total length after encrypted with AMLETool (including the 2K header)
    unsigned int  totalLenAfterEncrypted;

    unsigned char unused[1024 - SECUREBOOT_MAGIC_SIZE - 2 * sizeof(unsigned int)];
} T_EncryptBootImgInfo, *pT_EncryptBootImgInfo;

typedef struct SecureBootImgHdr {
    T_NormalBootImgHdr normalBootImgHdr;
    unsigned char reserve4Other[1024 - sizeof(normalBootImgHdr)];
    T_EncryptBootImgInfo encryptBootImgInfo;
} *pT_SecureBootImgHdr;

extern RecoveryUI *ui;

#endif  /* _SECURITY_H_ */
