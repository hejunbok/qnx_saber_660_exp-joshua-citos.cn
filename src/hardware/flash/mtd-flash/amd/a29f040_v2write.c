/*
 * $QNXLicenseC:
 * Copyright 2008, QNX Software Systems. 
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"). You 
 * may not reproduce, modify or distribute this software except in 
 * compliance with the License. You may obtain a copy of the License 
 * at: http://www.apache.org/licenses/LICENSE-2.0 
 * 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" basis, 
 * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
 *
 * This file may contain contributions from others, either as 
 * contributors under the License or as licensors under other terms.  
 * Please review this entire file for other proprietary rights or license 
 * notices, as well as the QNX Development Suite License Guide at 
 * http://licensing.qnx.com/license-guide/ for other information.
 * $
 */





#include <sys/f3s_mtd.h>

/*
 * Summary
 *
 * MTD Version:     2 only
 * Bus Width:       16-bit only
 * Buffered Writes: No
 * Note:            Depricated. Use f3s_a29f100_v2write() instead.
 *
 * Description
 *
 * Depricated. Slightly optimized for 16-bit flash. Use f3s_a29f100_v2write()
 * for a more general callout.
 */

int32_t f3s_a29f040_v2write(f3s_dbase_t * dbase,
                            f3s_access_t * access,
                            uint32_t flags,
                            uint32_t offset,
                            int32_t size,
                            uint8_t * buffer)
{
	/* Just a subset of the corresponding v2 function */
	return (amd_v2wordwrite(dbase, access, flags, offset, size, buffer,
	                        AMD_CMD_ADDR1_W16, AMD_CMD_ADDR2_W16));
}

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/6.6.0/trunk/hardware/flash/mtd-flash/amd/a29f040_v2write.c $ $Rev: 680332 $")
#endif
