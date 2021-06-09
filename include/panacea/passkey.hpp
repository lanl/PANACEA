#ifndef PANACEA_PASSKEY_H
#define PANACEA_PASSKEY_H
#pragma once

namespace panacea {

template <class T> class PassKey {
private:
  PassKey() {}
  PassKey(const PassKey &) {}
  PassKey &operator=(const PassKey &) = delete;
  friend T;
};
} // namespace panacea
#endif // PANACEA_PASSKEY_H
