#pragma once
#include "esphome/core/defines.h"
#ifdef USE_NOISE
#include <cstddef>
#include <cstdint>

// Forward declaration matching <noise/protocol/cipherstate.h>; keeps noise-c
// headers out of everything that includes noise.h.
extern "C" {
typedef struct NoiseCipherState_s NoiseCipherState;  // NOLINT(modernize-use-using)
}

namespace esphome::noise {

/** Session resume for the noise transports. */

static constexpr uint8_t RESUME_OFFER_VERSION = 0x01;
static constexpr uint8_t RESUME_ACCEPT_VERSION = 0x01;
static constexpr size_t RESUME_SESSION_ID_SIZE = 8;
static constexpr size_t RESUME_NONCE_SIZE = 16;
static constexpr size_t RESUME_MAC_SIZE = 16;
static constexpr size_t RESUME_SECRET_SIZE = 32;

// ClientHello body: version | session_id | client_nonce | offer_mac
static constexpr size_t RESUME_OFFER_SIZE = 1 + RESUME_SESSION_ID_SIZE + RESUME_NONCE_SIZE + RESUME_MAC_SIZE;  // 41
static constexpr size_t RESUME_OFFER_SESSION_ID_OFFSET = 1;
static constexpr size_t RESUME_OFFER_NONCE_OFFSET = RESUME_OFFER_SESSION_ID_OFFSET + RESUME_SESSION_ID_SIZE;
static constexpr size_t RESUME_OFFER_MAC_OFFSET = RESUME_OFFER_NONCE_OFFSET + RESUME_NONCE_SIZE;

// ServerHello trailing extension: version | server_nonce | confirm_mac
static constexpr size_t RESUME_ACCEPT_SIZE = 1 + RESUME_NONCE_SIZE + RESUME_MAC_SIZE;  // 33

struct ResumeTicket {
  uint8_t session_id[RESUME_SESSION_ID_SIZE];
  uint8_t secret[RESUME_SECRET_SIZE];
};

/// Fixed-slot RAM cache of single-use resume tickets.
class ResumeTicketCache {
 public:
  bool issue(ResumeTicket &out);
  bool try_accept(const uint8_t *offer, const uint8_t *prologue, size_t prologue_len, uint8_t *out_ext,
                  NoiseCipherState *&send_cipher, NoiseCipherState *&recv_cipher);
  void clear();

  static constexpr uint8_t SLOTS = 2;

 protected:
  ResumeTicket slots_[SLOTS];
  uint8_t used_mask_{0};
  uint8_t next_{0};
};

NoiseCipherState *resume_make_cipher(const uint8_t *key);

}  // namespace esphome::noise
#endif  // USE_NOISE
