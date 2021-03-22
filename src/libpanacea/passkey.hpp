#ifndef PANACEA_PRIVATE_PASSKEY_H
#define PANACEA_PRIVATE_PASSKEY_H
#pragma once

namespace panacea {

  template 
    class PassKey {
      private:
        PassKey() {}
        PassKey(const PassKey&) {}
        PassKey& operator=(const PassKey&) = delete;
        friend T;
    }; 
} 
#endif // PANACEA_PRIVATE_PASSKEY_H
