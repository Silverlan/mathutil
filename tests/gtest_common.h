// SPDX-FileCopyrightText: (c) 2020 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

#pragma once
#ifndef __GTEST_COMMON_H__
#define __GTEST_COMMON_H__

#define ANSI_TXT_GRN "\033[0;32m"
#define ANSI_TXT_MGT "\033[0;35m" //Magenta
#define ANSI_TXT_DFT "\033[0;0m" //Console default
#define GTEST_BOX "[     cout ] "
#define COUT_GTEST ANSI_TXT_GRN << GTEST_BOX //You could add the Default
#define COUT_GTEST_MGT COUT_GTEST << ANSI_TXT_MGT


#endif