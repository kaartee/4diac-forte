/************************************************************************************
 Copyright (c) 2024 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0

 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#ifndef ZEPHYR_IO_HANDLE_PWM_H
#define ZEPHYR_IO_HANDLE_PWM_H

#include "forte/io/mapper/io_observer.h"
#include "forte/datatypes/forte_dword.h"

#include <IODeviceController.h>

#include <zephyr/drivers/pwm.h>
#include <string>

class IOHandlePWM : public forte::io::IOHandle {
    using IOObserver = forte::io::IOObserver;
    using IOHandle = forte::io::IOHandle;
    using IOMapper = forte::io::IOMapper;
    using CIEC_ANY = forte::CIEC_ANY;
    using CIEC_DWORD = forte::CIEC_DWORD;

  public:
    IOHandlePWM(IODeviceController *paDeviceCtrl, std::string const &paId, const pwm_dt_spec *paPWMSpec);
    ~IOHandlePWM() override;

    void get(CIEC_ANY &paState) override;
    void set(const CIEC_ANY &paState) override;

    void reload();

  protected:
    void onObserver(IOObserver *paObserver) override;
    void dropObserver() override;

    const pwm_dt_spec *getPWMSpec() const {
      return mPWMSpec;
    }

  private:
    const pwm_dt_spec *const mPWMSpec;
    std::string mPwmPeriodId;
    IOHandle *mPwmPeriodHandle{nullptr};
    CIEC_DWORD mValue{0};
};

#endif /* ifndef ZEPHYR_IO_HANDLE_PWM_H */
