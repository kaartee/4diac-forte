/************************************************************************************
 Copyright (c) 2024 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0

 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#ifndef ZEPHYR_IO_HANDLE_GPIO_H
#define ZEPHYR_IO_HANDLE_GPIO_H

#include "forte/io/mapper/io_observer.h"

#include <IODeviceController.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

class IOHandleGPIO : public forte::io::IOHandle {
    using IOObserver = forte::io::IOObserver;
    using IOHandle = forte::io::IOHandle;
    using IOMapper = forte::io::IOMapper;
    using CIEC_ANY = forte::CIEC_ANY;

  public:
    IOHandleGPIO(IODeviceController *paDeviceCtrl,
                 IOMapper::Direction paDirection,
                 const gpio_dt_spec *paGPIOSpec,
                 gpio_flags_t paFlags);
    ~IOHandleGPIO() override;

    void get(CIEC_ANY &) override;
    void set(const CIEC_ANY &) override;

    bool equal();

  protected:
    void onObserver(IOObserver *paObserver) override;
    void dropObserver() override;

    const gpio_dt_spec *getGPIOSpec() const {
      return mGPIOSpec;
    }

  private:
    enum State { IDLE, NEWDATA, DESTROY };
    State mState{IDLE};
    const gpio_dt_spec *const mGPIOSpec;
    bool mUsingCallback{false};
    bool mValue{false};
    static void irq_callback(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
    static void work_callback(struct k_work *item);
    struct gpio_callback_context_t {
        struct gpio_callback gpio_cb_data;
        IOHandleGPIO *self;
    };
    struct gpio_callback_context_t mGPIOCallbackCtx;
    struct gpio_work_context_t {
        struct k_work work;
        IOHandleGPIO *self;
    };
    struct gpio_work_context_t mGPIOWorkCtx;
};

#endif /* ifndef ZEPHYR_IO_HANDLE_GPIO_H */
