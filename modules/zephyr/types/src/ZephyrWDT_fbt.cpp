/*************************************************************************
 *** Copyright (c) 2024, 2026 KT Elektronik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://eclipse.org
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.2.100.202609091534!
 ***
 *** Name: ZephyrWDT
 *** Description: Zephyr RTOS Hardware Watchdog Timer
 *** Version:
 ***     1.0: 2026-11-27/Dirk O. Kaar -  -
 *************************************************************************/

#include "forte/ZephyrWDT_fbt.h"

#include "forte/datatypes/forte_time.h"
#include "forte/forte_st_util.h"
#pragma region includes
#include <errno.h>
#include <zephyr/devicetree.h>
#pragma endregion includes

using namespace std::literals;
using namespace forte::literals;

namespace forte {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"INIT"_STRID, "REQ"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "CNF"_STRID};
    const auto cEventOutputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cDataInputNames = std::array{"QI"_STRID, "TIMEOUT"_STRID, "PAUSE_IN_DEBUG"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = cEventInputTypeIds,
        .mEONames = cEventOutputNames,
        .mEOTypeNames = cEventOutputTypeIds,
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };

#pragma region define WDT
#if DT_HAS_ALIAS(watchdog0)
#define FORTE_WDT_NODE DT_ALIAS(watchdog0)
#elif DT_HAS_CHOSEN(zephyr_watchdog)
#define FORTE_WDT_NODE DT_CHOSEN(zephyr_watchdog)
#elif DT_NODE_EXISTS(DT_NODELABEL(wdt0))
#define FORTE_WDT_NODE DT_NODELABEL(wdt0)
#elif DT_NODE_EXISTS(DT_NODELABEL(wdog1))
#define FORTE_WDT_NODE DT_NODELABEL(wdog1)
#else
#error "No suitable watchdog devicetree node found (define alias watchdog0)"
#endif

    const struct device *getWatchdogDevice() {
      return DEVICE_DT_GET(FORTE_WDT_NODE);
    }
#pragma endregion define WDT
  }

  DEFINE_FIRMWARE_FB(FORTE_ZephyrWDT, "ZephyrWDT"_STRID, TypeHash)

  FORTE_ZephyrWDT::FORTE_ZephyrWDT(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CFunctionBlock(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_QI(0_BOOL),
      var_TIMEOUT(2000000000_TIME),
      var_PAUSE_IN_DEBUG(1_BOOL),
      var_QO(0_BOOL),
      var_STATUS(""_STRING),
      conn_INITO(*this, 0),
      conn_CNF(*this, 1),
      conn_QI(nullptr),
      conn_TIMEOUT(nullptr),
      conn_PAUSE_IN_DEBUG(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS) {
  };

#pragma region destructor
  FORTE_ZephyrWDT::~FORTE_ZephyrWDT() {
    disableWatchdog();
  }
#pragma endregion destructor

  void FORTE_ZephyrWDT::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_QI = 0_BOOL;
    var_TIMEOUT = 2000000000_TIME;
    var_PAUSE_IN_DEBUG = 1_BOOL;
    var_QO = 0_BOOL;
    var_STATUS = ""_STRING;
  }

#pragma region define WDT
  void FORTE_ZephyrWDT::disableWatchdog() {
    if (mRunning && mWdtDev != nullptr) {
      int err = wdt_disable(mWdtDev);
      if (err < 0 && err != -EALREADY) {
        DEVLOG_WARNING("FORTE_ZephyrWDT: wdt_disable failed (%d)\n", err);
      }
    }
    mRunning = false;
    mChannelId = -1;
  }

  void FORTE_ZephyrWDT::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventINITID: {
        if (!var_QI) {
          disableWatchdog();
          var_QO = false_BOOL;
          var_STATUS = "Disabled"_STRING;
          sendOutputEvent(scmEventINITOID, paECET);
          break;
        }

        if (mRunning) {
          disableWatchdog();
        }

        mWdtDev = getWatchdogDevice();
        if (mWdtDev == nullptr || !device_is_ready(mWdtDev)) {
          DEVLOG_ERROR("FORTE_ZephyrWDT: watchdog device not ready\n");
          var_QO = false_BOOL;
          var_STATUS = "Device not ready"_STRING;
          sendOutputEvent(scmEventINITOID, paECET);
          break;
        }

        const auto timeoutMs = var_TIMEOUT.getInMilliSeconds();
        if (timeoutMs <= 0) {
          DEVLOG_ERROR("FORTE_ZephyrWDT: invalid timeout\n");
          var_QO = false_BOOL;
          var_STATUS = "Invalid timeout"_STRING;
          sendOutputEvent(scmEventINITOID, paECET);
          break;
        }

        struct wdt_timeout_cfg cfg{};
        cfg.window.min = 0U;
        cfg.window.max = static_cast<uint32_t>(timeoutMs);
        cfg.callback = nullptr;
        cfg.flags = WDT_FLAG_RESET_SOC;

        mChannelId = wdt_install_timeout(mWdtDev, &cfg);
        if (mChannelId < 0) {
          DEVLOG_ERROR("FORTE_ZephyrWDT: wdt_install_timeout failed (%d)\n", mChannelId);
          var_QO = false_BOOL;
          var_STATUS = "Install timeout failed"_STRING;
          sendOutputEvent(scmEventINITOID, paECET);
          break;
        }

        uint8_t options = var_PAUSE_IN_DEBUG ? WDT_OPT_PAUSE_HALTED_BY_DBG : 0U;
        int err = wdt_setup(mWdtDev, options);
        if (err == -ENOTSUP && options != 0U) {
          err = wdt_setup(mWdtDev, 0U);
        }
        if (err < 0) {
          DEVLOG_ERROR("FORTE_ZephyrWDT: wdt_setup failed (%d)\n", err);
          var_QO = false_BOOL;
          var_STATUS = "Setup failed"_STRING;
          sendOutputEvent(scmEventINITOID, paECET);
          break;
        }

        mRunning = true;
        var_QO = true_BOOL;
        var_STATUS = "OK"_STRING;
        sendOutputEvent(scmEventINITOID, paECET);
        break;
      }

      case scmEventREQID: {
        if (!var_QI || !mRunning || mWdtDev == nullptr || mChannelId < 0) {
          var_QO = false_BOOL;
          var_STATUS = "Not initialized"_STRING;
          sendOutputEvent(scmEventCNFID, paECET);
          break;
        }

        int err = wdt_feed(mWdtDev, mChannelId);
        if (err < 0) {
          DEVLOG_ERROR("FORTE_ZephyrWDT: wdt_feed failed (%d)\n", err);
          var_QO = false_BOOL;
          var_STATUS = "Feed failed"_STRING;
          sendOutputEvent(scmEventCNFID, paECET);
          break;
        }

        var_QO = true_BOOL;
        var_STATUS = "OK"_STRING;
        sendOutputEvent(scmEventCNFID, paECET);
        break;
      }

      default:
        break;
    }
  }
#pragma endregion define WDT

  void FORTE_ZephyrWDT::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITID: {
        readData(0, var_QI, conn_QI);
        readData(1, var_TIMEOUT, conn_TIMEOUT);
        readData(2, var_PAUSE_IN_DEBUG, conn_PAUSE_IN_DEBUG);
        break;
      }
      case scmEventREQID: {
        readData(0, var_QI, conn_QI);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_ZephyrWDT::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITOID: {
        writeData(3, var_QO, conn_QO);
        writeData(4, var_STATUS, conn_STATUS);
        break;
      }
      case scmEventCNFID: {
        writeData(3, var_QO, conn_QO);
        writeData(4, var_STATUS, conn_STATUS);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_ZephyrWDT::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_TIMEOUT;
      case 2: return &var_PAUSE_IN_DEBUG;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_ZephyrWDT::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *FORTE_ZephyrWDT::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_ZephyrWDT::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_TIMEOUT;
      case 2: return &conn_PAUSE_IN_DEBUG;
    }
    return nullptr;
  }

  CDataConnection *FORTE_ZephyrWDT::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }
}
