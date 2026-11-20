/*************************************************************************
 *** Copyright (c) 2024 KT Elektronik GmbH
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.2.100.202609091534!
 ***
 *** Name: ZephyrIO
 *** Description: Template for Modular IO with boards running Zephyr OS
 *** Version:
 ***     1.0: 2024-01-12/Dirk Kaar -  -
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_time.h"
#include "forte/forte_st_util.h"
#pragma region includes
#include <types/include/forte/ZephyrIOBase.h>
#pragma endregion includes

namespace forte {
#pragma region base class spec
  class FORTE_ZephyrIO final : public FORTE_ZephyrIOBase {
#pragma endregion base class spec
      DECLARE_FIRMWARE_FB(FORTE_ZephyrIO)

    private:
      static const TEventID scmEventINITOID = 0;
      static const TEventID scmEventINITID = 0;

#pragma region remove executeEvent()
      // using FORTE_ZephyrIOBase::executeEvent
#pragma endregion remove executeEvent()

      void readInputData(TEventID paEIID) override;
      void writeOutputData(TEventID paEIID) override;
      void setInitialValues() override;

#pragma region define IOs
      static constexpr size_t numberOfIOs = 0;

      void setConfig() override;

    protected:
      void onStartup(CEventChainExecutionThread *const paECET) override;
#pragma endregion define IOs

    public:
      FORTE_ZephyrIO(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_QI;
      CIEC_TIME var_UpdateInterval;

      CIEC_BOOL var_QO;
      CIEC_STRING var_STATUS;

      CEventConnection conn_INITO;

      CDataConnection *conn_QI;
      CDataConnection *conn_UpdateInterval;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_STRING> conn_STATUS;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_INIT(const CIEC_BOOL &paQI, const CIEC_TIME &paUpdateInterval, CAnyBitOutputParameter<CIEC_BOOL> paQO, COutputParameter<CIEC_STRING> paSTATUS) {
        COutputGuard guard_QO(paQO);
        COutputGuard guard_STATUS(paSTATUS);
        var_QI = paQI;
        var_UpdateInterval = paUpdateInterval;
        executeEvent(scmEventINITID, nullptr);
        *paQO = var_QO;
        *paSTATUS = var_STATUS;
      }

      void operator()(const CIEC_BOOL &paQI, const CIEC_TIME &paUpdateInterval, CAnyBitOutputParameter<CIEC_BOOL> paQO, COutputParameter<CIEC_STRING> paSTATUS) {
        evt_INIT(std::forward<const CIEC_BOOL &>(paQI), std::forward<const CIEC_TIME &>(paUpdateInterval), std::forward<CAnyBitOutputParameter<CIEC_BOOL>>(paQO), std::forward<COutputParameter<CIEC_STRING>>(paSTATUS));
      }
  };
}
