/************************************************************************************
 Copyright (c) 2023 KT Elektronik GmbH
 This program and the accompanying materials are made available under the
 terms of the Eclipse Public License 2.0 which is available at
 http://www.eclipse.org/legal/epl-2.0.

 SPDX-License-Identifier: EPL-2.0

 Contributors:
  Dirk Kaar - initial API and implementation and/or initial documentation
 ************************************************************************************/

#ifndef SRC_ARCH_ZEPHYR_THREAD_H_
#define SRC_ARCH_ZEPHYR_THREAD_H_

#include "forte/datatype.h"
#include "forte/util/devlog.h"
#include "forte/datatypes/forte_time.h"
#include "forte/arch/threadbase.h"
#include "forte/arch/forte_sync.h"

#include <zephyr/kernel.h>

// Allow to set a special stack size for forte threads
#ifndef configMINIMAL_STACK_SIZE_FORTE
#define configMINIMAL_STACK_SIZE_FORTE 2048
#endif

namespace forte::arch {

  class CZephyrThread;

  using TZephyrThreadPtr = CZephyrThread *;

  /** \defgroup Zephyr-HAL Zephyr FORTE Hardware Abstraction Layer
   * \ingroup FORTE-HAL
   * \brief The FORTE-HAL implementation for the Zephyr operating system.
   */
  /*! \ingroup Zephyr-HAL
   * \brief This class is a wrapper class for Zephyr multitasking support
   */
  class CZephyrThread : public CThreadBase<k_tid_t> {
    public:
      /*! \brief Constructor of the Thread class
       *
       * Does all the necessary steps in order to get the thread running with the start()-method
       * @param paStackSize the Size of the stack the thread is allowed to use. This class will
       *        allocate the stack size in bytes from the heap
       */
      explicit CZephyrThread(long paStackSize = configMINIMAL_STACK_SIZE_FORTE);

      /*! \brief Stops and destroys thread.
       *
       * Will stop the execution if running and destroy the thread including all system specific data.
       */
      ~CZephyrThread() override;

      //! Set the deadline of the thread.
      void setDeadline(const forte::CIEC_TIME &paVal) override final;

      /*! \brief Sleep the calling thread
       *
       * @param paMilliSeconds The milliseconds for the thread to sleep
       */
      static void sleepThread(unsigned int paMilliSeconds);

    protected:
      void setPriority(int paPriority) {
        k_thread_priority_set(thread, paPriority);
      }

    private:
      /*!\brief Function that is given to the system thread support that should be called for the thread.
       *
       * This function will call the run method of the thread instance.
       * Zephyr requirement that this has to be a static function.
       */
      static void threadFunction(void *arg1, void *arg2, void *arg3);

      /*! \brief Creates a new thread.
       *
       * With this function all the setup things for a new thread are done. The thread created is initially
       * suspended and with the start() method the execution of the code in the run() method is started.
       */
      TThreadHandleType createThread(long paStackSize) override;

      k_thread_stack_t *stack{0};
      struct k_thread *thread{0};
  };

  using CThread = CZephyrThread;

} // namespace forte::arch

#endif /* SRC_ARCH_ZEPHYR_THREAD_H_ */
