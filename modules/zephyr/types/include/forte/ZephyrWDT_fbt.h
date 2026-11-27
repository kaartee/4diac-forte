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

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_time.h"
#include "forte/forte_st_util.h"
#pragma region includes
#include <zephyr/device.h>
#include <zephyr/drivers/watchdog.h>
#pragma endregion includes

namespace forte {
  class FORTE_ZephyrWDT final : public CFunctionBlock {
      DECLARE_FIRMWARE_FB(FORTE_ZephyrWDT)

    private:
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventCNFID = 1;
      static const TEventID scmEventINITID = 0;
      static const TEventID scmEventREQID = 1;

      void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

#pragma region define WDT
      const struct device *mWdtDev{nullptr};
      int mChannelId{-1};
      bool mRunning{false};

      void disableWatchdog();
#pragma endregion define WDT

    public:
      FORTE_ZephyrWDT(StringId paInstanceNameId, CFBContainer &paContainer);
#pragma region destructor
      ~FORTE_ZephyrWDT() override;
#pragma endregion destructor

      CIEC_BOOL var_QI;
      CIEC_TIME var_TIMEOUT;
      CIEC_BOOL var_PAUSE_IN_DEBUG;

      CIEC_BOOL var_QO;
      CIEC_STRING var_STATUS;

      CEventConnection conn_INITO;
      CEventConnection conn_CNF;

      CDataConnection *conn_QI;
      CDataConnection *conn_TIMEOUT;
      CDataConnection *conn_PAUSE_IN_DEBUG;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_STRING> conn_STATUS;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_INIT(const CIEC_BOOL &paQI, const CIEC_TIME &paTIMEOUT, const CIEC_BOOL &paPAUSE_IN_DEBUG, CAnyBitOutputParameter<CIEC_BOOL> paQO, COutputParameter<CIEC_STRING> paSTATUS) {
        COutputGuard guard_QO(paQO);
        COutputGuard guard_STATUS(paSTATUS);
        var_QI = paQI;
        var_TIMEOUT = paTIMEOUT;
        var_PAUSE_IN_DEBUG = paPAUSE_IN_DEBUG;
        executeEvent(scmEventINITID, nullptr);
        *paQO = var_QO;
        *paSTATUS = var_STATUS;
      }

      void evt_REQ(const CIEC_BOOL &paQI, const CIEC_TIME &paTIMEOUT, const CIEC_BOOL &paPAUSE_IN_DEBUG, CAnyBitOutputParameter<CIEC_BOOL> paQO, COutputParameter<CIEC_STRING> paSTATUS) {
        COutputGuard guard_QO(paQO);
        COutputGuard guard_STATUS(paSTATUS);
        var_QI = paQI;
        var_TIMEOUT = paTIMEOUT;
        var_PAUSE_IN_DEBUG = paPAUSE_IN_DEBUG;
        executeEvent(scmEventREQID, nullptr);
        *paQO = var_QO;
        *paSTATUS = var_STATUS;
      }

      void operator()(const CIEC_BOOL &paQI, const CIEC_TIME &paTIMEOUT, const CIEC_BOOL &paPAUSE_IN_DEBUG, CAnyBitOutputParameter<CIEC_BOOL> paQO, COutputParameter<CIEC_STRING> paSTATUS) {
        evt_INIT(std::forward<const CIEC_BOOL &>(paQI), std::forward<const CIEC_TIME &>(paTIMEOUT), std::forward<const CIEC_BOOL &>(paPAUSE_IN_DEBUG), std::forward<CAnyBitOutputParameter<CIEC_BOOL>>(paQO), std::forward<COutputParameter<CIEC_STRING>>(paSTATUS));
      }
  };
}
