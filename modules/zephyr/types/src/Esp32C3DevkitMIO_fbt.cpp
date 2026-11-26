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

#include "forte/Esp32C3DevkitMIO_fbt.h"

#include "forte/datatypes/forte_time.h"
#include "forte/forte_st_util.h"
#pragma region includes
#include <IOHandleGPIODescriptor.h>
#include <IOHandleADCDescriptor.h>
#include <IOHandlePWMDescriptor.h>
#include "IODeviceController.h"
#pragma endregion includes

using namespace std::literals;
using namespace forte::literals;

namespace forte {
  namespace {
    constexpr std::string_view TypeHash =""sv;

    const auto cEventInputNames = std::array{"INIT"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID};
    const auto cEventOutputTypeIds = std::array{"EInit"_STRID};
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
  }

  DEFINE_FIRMWARE_FB(FORTE_Esp32C3DevkitMIO, "Esp32C3DevkitMIO"_STRID, TypeHash)

#pragma region base class spec
  FORTE_Esp32C3DevkitMIO::FORTE_Esp32C3DevkitMIO(const StringId paInstanceNameId, CFBContainer &paContainer) :
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
      conn_QI(nullptr),
      conn_LED0(nullptr),
      conn_SW0(nullptr),
      conn_ADC_CH_0(nullptr),
      conn_PWM(nullptr),
      conn_UpdateInterval(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS) {
  };
#pragma endregion base class spec

  void FORTE_Esp32C3DevkitMIO::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_QI = 0_BOOL;
    var_LED0 = ""_STRING;
    var_SW0 = ""_STRING;
    var_ADC_CH_0 = ""_STRING;
    var_PWM = ""_STRING;
    var_UpdateInterval = 40000000_TIME;
    var_QO = 0_BOOL;
    var_STATUS = ""_STRING;
  }

#pragma region remove executeEvent()
  // using FORTE_ZephyrIOBase::executeEvent
#pragma endregion remove executeEvent()

  void FORTE_Esp32C3DevkitMIO::readInputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventINITID: {
        readData(0, var_QI, conn_QI);
        readData(5, var_UpdateInterval, conn_UpdateInterval);
        readData(1, var_LED0, conn_LED0);
        readData(2, var_SW0, conn_SW0);
        readData(3, var_ADC_CH_0, conn_ADC_CH_0);
        readData(4, var_PWM, conn_PWM);
        break;
      }
      default:
        break;
    }
  }

  void FORTE_Esp32C3DevkitMIO::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventINITOID: {
        writeData(6, var_QO, conn_QO);
        writeData(7, var_STATUS, conn_STATUS);
        break;
      }
      default:
        break;
    }
  }

  CIEC_ANY *FORTE_Esp32C3DevkitMIO::getDI(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_QI;
      case 1: return &var_LED0;
      case 2: return &var_SW0;
      case 3: return &var_ADC_CH_0;
      case 4: return &var_PWM;
      case 5: return &var_UpdateInterval;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_Esp32C3DevkitMIO::getDO(const size_t paIndex) {
    switch(paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *FORTE_Esp32C3DevkitMIO::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_INITO;
    }
    return nullptr;
  }

  CDataConnection **FORTE_Esp32C3DevkitMIO::getDIConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_LED0;
      case 2: return &conn_SW0;
      case 3: return &conn_ADC_CH_0;
      case 4: return &conn_PWM;
      case 5: return &conn_UpdateInterval;
    }
    return nullptr;
  }

  CDataConnection *FORTE_Esp32C3DevkitMIO::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }

#pragma region define IOs
  namespace {
    struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
    struct gpio_dt_spec sw0 = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
    struct gpio_spec_mod {
        const gpio_dt_spec *spec;
        gpio_flags_t flags;
    };
    const gpio_spec_mod gpio_spec_mods[] = {
        {&led0, GPIO_OUTPUT},
        {&sw0, GPIO_INPUT | GPIO_INT_EDGE_BOTH},
    };

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || !DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

    /* Data of ADC io-channels specified in devicetree. */
    const struct adc_dt_spec adc_channels[] = {
        DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels, DT_SPEC_AND_COMMA)};

    struct pwm_dt_spec pwm0 = PWM_DT_SPEC_GET(DT_ALIAS(pwm0));
    pwm_dt_spec *pwm_specs[] = {
        &pwm0,
    };
  }; // namespace

  void FORTE_Esp32C3DevkitMIO::setConfig() {
    ::IODeviceController::Config config;
    config.updateInterval =
        static_cast<CIEC_TIME *>(getDI(FORTE_ZephyrIOBase::initialDIOffset + FORTE_Esp32C3DevkitMIO::numberOfGPIOs +
                                       FORTE_Esp32C3DevkitMIO::numberOfADCChannels +
                                       FORTE_Esp32C3DevkitMIO::numberOfPWMs))
            ->getInMilliSeconds();
    getDeviceController()->setConfig(&config);
  }

  void FORTE_Esp32C3DevkitMIO::onStartup(CEventChainExecutionThread *const paECET) {
    // Initialize handles
    for (size_t i = 0; i < FORTE_Esp32C3DevkitMIO::numberOfGPIOs; i++) {
      const auto id = std::string(*static_cast<CIEC_STRING *>(getDI(FORTE_ZephyrIOBase::initialDIOffset + i)));
      if (!gpio_spec_mods[i].spec) {
        DEVLOG_ERROR("FORTE_Esp32C3DevkitMIO::onStartup: id %s, device spec is null\n", id.c_str());
        continue;
      }
      IOHandleGPIODescriptor descr(id, (gpio_spec_mods[i].flags & GPIO_OUTPUT) ? IOMapper::Out : IOMapper::In,
                                   gpio_spec_mods[i].spec, gpio_spec_mods[i].flags);
      initHandle(descr);
    }
    if (FORTE_Esp32C3DevkitMIO::numberOfADCChannels != ARRAY_SIZE(adc_channels)) {
      DEVLOG_ERROR("FORTE_Esp32C3DevkitMIO::onStartup: ADC channel count configuration mismatch\n");
    } else
      for (size_t i = 0; i < FORTE_Esp32C3DevkitMIO::numberOfADCChannels; i++) {
        const auto id = std::string(*static_cast<CIEC_STRING *>(
            getDI(FORTE_ZephyrIOBase::initialDIOffset + FORTE_Esp32C3DevkitMIO::numberOfGPIOs + i)));
        if (!adc_is_ready_dt(&adc_channels[i])) {
          DEVLOG_ERROR("FORTE_Esp32C3DevkitMIO::onStartup: id %s ADC controller device %s not ready\n", id.c_str(),
                       adc_channels[i].dev->name);
          continue;
        }
        IOHandleADCDescriptor descr(id, &adc_channels[i]);
        initHandle(descr);
      }
    for (size_t i = 0; i < FORTE_Esp32C3DevkitMIO::numberOfPWMs; i++) {
      const auto id = std::string(*static_cast<CIEC_STRING *>(getDI(FORTE_ZephyrIOBase::initialDIOffset +
                                                                    FORTE_Esp32C3DevkitMIO::numberOfGPIOs +
                                                                    FORTE_Esp32C3DevkitMIO::numberOfADCChannels + i)));
      if (!pwm_is_ready_dt(pwm_specs[i])) {
        DEVLOG_ERROR("FORTE_Esp32C3DevkitMIO::onStartup: id %s, PWM device %s not ready\n", id.c_str(),
                     pwm_specs[i]->dev->name);
        continue;
      }
      IOHandlePWMDescriptor descr(id, pwm_specs[i]);
      initHandle(descr);
    }

    FORTE_ZephyrIOBase::onStartup(paECET);
  }
#pragma endregion define IOs

}
