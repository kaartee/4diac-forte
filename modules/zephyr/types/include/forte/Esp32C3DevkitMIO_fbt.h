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
 *** Name: Esp32C3DevkitMIO
 *** Description: Modular IO with Espressif ESP32-C3-DevKitM-1 board
 *** Version:
 ***     1.0: 2026-11-26/Dirk O. Kaar -  -
 *************************************************************************/

#pragma once

#include "forte/funcbloc.h"
#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_time.h"
#include "forte/forte_st_util.h"
#pragma region includes
#include <forte/ZephyrIOBase.h>
#pragma endregion includes

namespace forte {
#pragma region base class spec
  class FORTE_Esp32C3DevkitMIO final : public FORTE_ZephyrIOBase {
#pragma endregion base class spec
      DECLARE_FIRMWARE_FB(FORTE_Esp32C3DevkitMIO)

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
      static constexpr size_t numberOfGPIOs = 2;
      static constexpr size_t numberOfADCChannels = 1;
      static constexpr size_t numberOfPWMs = 1;

      void setConfig() override;

    protected:
      void onStartup(CEventChainExecutionThread *const paECET) override;
#pragma endregion define IOs

    public:
      FORTE_Esp32C3DevkitMIO(StringId paInstanceNameId, CFBContainer &paContainer);

      CIEC_BOOL var_QI;
      CIEC_STRING var_LED0;
      CIEC_STRING var_SW0;
      CIEC_STRING var_ADC_CH_0;
      CIEC_STRING var_PWM;
      CIEC_TIME var_UpdateInterval;

      CIEC_BOOL var_QO;
      CIEC_STRING var_STATUS;

      CEventConnection conn_INITO;

      CDataConnection *conn_QI;
      CDataConnection *conn_LED0;
      CDataConnection *conn_SW0;
      CDataConnection *conn_ADC_CH_0;
      CDataConnection *conn_PWM;
      CDataConnection *conn_UpdateInterval;

      COutDataConnection<CIEC_BOOL> conn_QO;
      COutDataConnection<CIEC_STRING> conn_STATUS;

      CIEC_ANY *getDI(size_t) override;
      CIEC_ANY *getDO(size_t) override;
      CEventConnection *getEOConUnchecked(TPortId) override;
      CDataConnection **getDIConUnchecked(TPortId) override;
      CDataConnection *getDOConUnchecked(TPortId) override;

      void evt_INIT(const CIEC_BOOL &paQI, const CIEC_STRING &paLED0, const CIEC_STRING &paSW0, const CIEC_STRING &paADC_CH_0, const CIEC_STRING &paPWM, const CIEC_TIME &paUpdateInterval, CAnyBitOutputParameter<CIEC_BOOL> paQO, COutputParameter<CIEC_STRING> paSTATUS) {
        COutputGuard guard_QO(paQO);
        COutputGuard guard_STATUS(paSTATUS);
        var_QI = paQI;
        var_LED0 = paLED0;
        var_SW0 = paSW0;
        var_ADC_CH_0 = paADC_CH_0;
        var_PWM = paPWM;
        var_UpdateInterval = paUpdateInterval;
        executeEvent(scmEventINITID, nullptr);
        *paQO = var_QO;
        *paSTATUS = var_STATUS;
      }

      void operator()(const CIEC_BOOL &paQI, const CIEC_STRING &paLED0, const CIEC_STRING &paSW0, const CIEC_STRING &paADC_CH_0, const CIEC_STRING &paPWM, const CIEC_TIME &paUpdateInterval, CAnyBitOutputParameter<CIEC_BOOL> paQO, COutputParameter<CIEC_STRING> paSTATUS) {
        evt_INIT(std::forward<const CIEC_BOOL &>(paQI), std::forward<const CIEC_STRING &>(paLED0), std::forward<const CIEC_STRING &>(paSW0), std::forward<const CIEC_STRING &>(paADC_CH_0), std::forward<const CIEC_STRING &>(paPWM), std::forward<const CIEC_TIME &>(paUpdateInterval), std::forward<CAnyBitOutputParameter<CIEC_BOOL>>(paQO), std::forward<COutputParameter<CIEC_STRING>>(paSTATUS));
      }
  };
}
