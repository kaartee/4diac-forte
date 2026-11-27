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
 *** Name: Teensy41IO
 *** Description: PJRC Teensy 4.1 Hardware I/O Configuration
 *** Version:
 ***     1.0: 2026-11-27/Dirk O. Kaar -  -
 *************************************************************************/

#include "forte/Teensy41IO_fbt.h"

#include "forte/datatypes/forte_time.h"
#include "forte/forte_st_util.h"
#pragma region includes
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/devicetree.h>
#include "IOHandleGPIODescriptor.h"
#include "IOHandleADCDescriptor.h"
#include "IOHandlePWMDescriptor.h"
#include "IODeviceController.h"
#pragma endregion includes

using namespace std::literals;
using namespace forte::literals;

namespace forte {
  namespace {
    constexpr std::string_view TypeHash = ""sv;

    const auto cEventInputNames = std::array{"INIT"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "IND"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID};
    const auto cDataInputNames = std::array{"QI"_STRID, "LED0"_STRID, "SW0"_STRID, "ADC_CH_0"_STRID, "PWM"_STRID, "UpdateInterval"_STRID};
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

#pragma region define handles
    const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
    const struct gpio_dt_spec sw0 = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
    const struct adc_dt_spec adc_ch_0 = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 0);
    const struct pwm_dt_spec pwm0 = PWM_DT_SPEC_GET(DT_ALIAS(pwm0));
#pragma endregion define handles
  }

  DEFINE_FIRMWARE_FB(FORTE_Teensy41IO, "Teensy41IO"_STRID, TypeHash)

  FORTE_Teensy41IO::FORTE_Teensy41IO(const StringId paInstanceNameId, CFBContainer &paContainer) :
      FORTE_ZephyrIOBase(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_QI(0_BOOL),
      var_LED0(""_STRING),
      var_SW0(""_STRING),
      var_ADC_CH_0(""_STRING),
      var_PWM(""_STRING),
      var_UpdateInterval(40000000_TIME),
      var_QO(0_BOOL),
      var_STATUS(""_STRING),
      conn_INITO(*this, 0),
      conn_IND(*this, 1),
      conn_QI(nullptr),
      conn_LED0(nullptr),
      conn_SW0(nullptr),
      conn_ADC_CH_0(nullptr),
      conn_PWM(nullptr),
      conn_UpdateInterval(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS) {
  };

  void FORTE_Teensy41IO::setInitialValues() {
    var_QI = 0_BOOL;
    var_LED0 = ""_STRING;
    var_SW0 = ""_STRING;
    var_ADC_CH_0 = ""_STRING;
    var_PWM = ""_STRING;
    var_UpdateInterval = 40000000_TIME;
    var_QO = 0_BOOL;
    var_STATUS = ""_STRING;
  }

#pragma region define handles
  void FORTE_Teensy41IO::onStartup(CEventChainExecutionThread *const paECET) {
    FORTE_ZephyrIOBase::initialise({
        IOHandleGPIODescriptor(var_LED0.getStorage(), forte::io::IOMapper::Out, &led0, GPIO_OUTPUT_INACTIVE),
        IOHandleGPIODescriptor(var_SW0.getStorage(), forte::io::IOMapper::In, &sw0, GPIO_INPUT | GPIO_PULL_UP),
        IOHandleADCDescriptor(var_ADC_CH_0.getStorage(), forte::io::IOMapper::In, &adc_ch_0),
        IOHandlePWMDescriptor(var_PWM.getStorage(), forte::io::IOMapper::Out, &pwm0),
    }, var_UpdateInterval);
    FORTE_ZephyrIOBase::onStartup(paECET);
  }
#pragma endregion define handles

  void FORTE_Teensy41IO::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventINITID:
        if (var_QI) {
          onStartup(paECET);
        } else {
          onStop(paECET);
        }
        sendOutputEvent(scmEventINITOID, paECET);
        break;
    }
  }

  void FORTE_Teensy41IO::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITID: {
        readData(0, var_QI, conn_QI);
        readData(1, var_LED0, conn_LED0);
        readData(2, var_SW0, conn_SW0);
        readData(3, var_ADC_CH_0, conn_ADC_CH_0);
        readData(4, var_PWM, conn_PWM);
        readData(5, var_UpdateInterval, conn_UpdateInterval);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_Teensy41IO::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITOID: {
        writeData(6, var_QO, conn_QO);
        writeData(7, var_STATUS, conn_STATUS);
        break;
      }
      case scmEventINDID: {
        writeData(6, var_QO, conn_QO);
        writeData(7, var_STATUS, conn_STATUS);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_Teensy41IO::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_LED0;
      case 2: return &var_SW0;
      case 3: return &var_ADC_CH_0;
      case 4: return &var_PWM;
      case 5: return &var_UpdateInterval;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_Teensy41IO::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *FORTE_Teensy41IO::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_IND;
    }
    return nullptr;
  }

  CDataConnection **FORTE_Teensy41IO::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_LED0;
      case 2: return &conn_SW0;
      case 3: return &conn_ADC_CH_0;
      case 4: return &conn_PWM;
      case 5: return &conn_UpdateInterval;
    }
    return nullptr;
  }

  CDataConnection *FORTE_Teensy41IO::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }
}
