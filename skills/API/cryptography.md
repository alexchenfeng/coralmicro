Cryptography
============

The following functions are convenience helpers for basic usage of the a71ch
crypto chip with a more modern C++ style. For the full a71châs API, see the
reference manual at [nxp.com/a71ch](https://www.nxp.com/a71ch).

Note

The following APIs can be used together with the full a71ch APIs in
`third_party/a71ch-crypto-support/`, however,
you must always call [`coralmicro::A71ChInit()`](#_CPPv4N10coralmicro9A71ChInitEv "coralmicro::A71ChInit") first.

For example, this code shows how to use a variety of the available APIs
(from `examples/security/`):

```
namespace coralmicro {
namespace {
void Main() {
  printf("Security Example!\r\n");
  // Turn on Status LED to show the board is on.
  LedSet(Led::kStatus, true);

  // Initializes the A71 chip.
  if (!coralmicro::A71ChInit()) {
    printf("Failed to initializes the a71ch chip\r\n");
    vTaskSuspend(nullptr);
  }

  // Get the A71's unique id.
  if (auto a71ch_uid = coralmicro::A71ChGetUId(); a71ch_uid.has_value()) {
    printf("A71 Unique ID: %s\r\n",
           coralmicro::StrToHex(a71ch_uid.value()).c_str());
  } else {
    printf("Failed to retrieve A71's unique ID\r\n");
    vTaskSuspend(nullptr);
  }

  // Get random bytes from the A71.
  for (uint8_t random_len = 8; random_len <= 64; random_len <<= 1) {
    if (auto random_bytes = coralmicro::A71ChGetRandomBytes(random_len);
        random_bytes.has_value()) {
      printf("A71 Random %d Bytes: %s\r\n", random_len,
             coralmicro::StrToHex(random_bytes.value()).c_str());
    } else {
      printf("Failed to get random bytes from A71\r\n");
      vTaskSuspend(nullptr);
    }
  }

  // The storage index of the ecc key pair to sign and verify.
  constexpr uint8_t kKeyIdx = 0;

  // Get the public ECC key at index 0 from the A71 chip.
  auto maybe_ecc_pub_key = coralmicro::A71ChGetEccPublicKey(kKeyIdx);
  if (maybe_ecc_pub_key.has_value()) {
    printf("A71 ECC public key %d: %s\r\n", kKeyIdx,
           coralmicro::StrToHex(maybe_ecc_pub_key.value()).c_str());
  } else {
    printf("Failed to get A71 ECC public key %d\r\n", kKeyIdx);
    vTaskSuspend(nullptr);
  }

  // Get the sha256 and ecc signature for this model file.
  std::vector<uint8_t> model;
  constexpr char kModelPath[] = "/models/testconv1-edgetpu.tflite";
  if (!coralmicro::LfsReadFile(kModelPath, &model)) {
    printf("%s missing\r\n", kModelPath);
    vTaskSuspend(nullptr);
  }
  auto maybe_sha = coralmicro::A71ChGetSha256(model);
  if (maybe_sha.has_value()) {
    printf("testconv1-edgetpu.tflite sha: %s\r\n",
           coralmicro::StrToHex(maybe_sha.value()).c_str());
  } else {
    printf("failed to generate sha256 for testconv1-edgetpu.tflite\r\n");
    vTaskSuspend(nullptr);
  }

  // Get signature for this model with the key public key in index 0.
  const auto& sha = maybe_sha.value();
  auto maybe_signature = coralmicro::A71ChGetEccSignature(kKeyIdx, sha);
  if (maybe_signature.has_value()) {
    printf("Signature: %s\r\n",
           coralmicro::StrToHex(maybe_signature.value()).c_str());
  } else {
    printf("failed to get ecc signature\r\n");
    vTaskSuspend(nullptr);
  }

  // Verifying the signature using the key storage index.
  const auto& signature = maybe_signature.value();
  printf(
      "EccVerify: %s\r\n",
      coralmicro::A71ChEccVerify(kKeyIdx, sha, signature) ? "success" : "fail");

  // Verifying the signature using the key.
  const auto& ecc_pub_key = maybe_ecc_pub_key.value();
  printf("EccVerifyWithKey: %s\r\n",
         coralmicro::A71ChEccVerifyWithKey(ecc_pub_key, sha, signature)
             ? "success"
             : "fail");

  printf(
      "\r\nSee examples/security/README.md for instructions to verify "
      "manually.\r\n");
}
}  // namespace
}  // namespace coralmicro

extern "C" void app_main(void* param) {
  (void)param;
  coralmicro::Main();
  vTaskSuspend(nullptr);
}
```

[[a71ch.h source]](https://github.com/google-coral/coralmicro/blob/main/libs/a71ch/a71ch.h)

*namespace* `coralmicro`
:   Functions

    bool `A71ChInit`()[Â¶](#_CPPv4N10coralmicro9A71ChInitEv "Permalink to this definition")
    :   Initializes the a71ch module.

        You must call this before using any of the following a71ch helper functions or the direct a71ch APIs (from `third_party/a71ch/`). You must also call this before using SSL features in Mbed TLS (from `third_party/nxp/rt1176-sdk/middleware/mbedtls/`) or before requesting an HTTPS URL with Curl (from `libs/curl/curl.h`).

    std::optional<std::string> `A71ChGetUId`()[Â¶](#_CPPv4N10coralmicro11A71ChGetUIdEv "Permalink to this definition")
    :   Gets the uid of the a71ch module.

        Returns
        :   The unique id of this a71ch module or std::nullopt.

    std::optional<std::string> `A71ChGetRandomBytes`(uint8\_t *num\_bytes*)[Â¶](#_CPPv4N10coralmicro19A71ChGetRandomBytesE7uint8_t "Permalink to this definition")
    :   Gets the random bytes from the A71.

        Parameters
        :   **num\_bytes** â The number of random bytes.

        Returns
        :   The random bytes or std::nullopt on failure.

    std::optional<std::string> `A71ChGetEccPublicKey`(uint8\_t *key\_idx*)[Â¶](#_CPPv4N10coralmicro20A71ChGetEccPublicKeyE7uint8_t "Permalink to this definition")
    :   Gets a public Elliptical Curve key from the A71.

        Parameters
        :   **key\_idx** â The key storage index to obtain the key. Note: The a71ch chip can only hold up to 4 key pairs, so this function will fail if key\_idx > 3. See the manual referenced above for more info.

        Returns
        :   The public ECC key in index key\_idx or std::nullopt if failed to retrieve.

    std::optional<std::string> `A71ChGetSha256`(uint8\_t \**data*, uint16\_t *data\_len*)[Â¶](#_CPPv4N10coralmicro14A71ChGetSha256EP7uint8_t8uint16_t "Permalink to this definition")
    :   Gets the sha256 hash for a buffer of data.

        Parameters
        :   * **data** â The buffer to the raw data to generate the hash.
            * **data\_len** â The length of the data buffer.

        Returns
        :   The sha256 hash for data or std::nullopt on failure.

    std::optional<std::string> `A71ChGetSha256`(*const* std::vector<uint8\_t> &*data*)[Â¶](#_CPPv4N10coralmicro14A71ChGetSha256ERKNSt6vectorI7uint8_tEE "Permalink to this definition")
    :   Gets the sha256 hash for a vector of data.

        Parameters
        :   **data** â The raw data to generate the hash.

        Returns
        :   The sha256 hash for data or std::nullopt on failure.

    std::optional<std::string> `A71ChGetEccSignature`(uint8\_t *key\_idx*, *const* uint8\_t \**sha*, uint16\_t *sha\_len*)[Â¶](#_CPPv4N10coralmicro20A71ChGetEccSignatureE7uint8_tPK7uint8_t8uint16_t "Permalink to this definition")
    :   Gets the Elliptical Curve signature for a hash.

        Parameters
        :   * **key\_idx** â The key storage index of the public key to sign this hash.
            * **sha** â The buffer containing the sha256 hash of some data to sign.
            * **sha\_len** â The length of the sha.

        Returns
        :   The Ecc Signature or std::nullopt on failure.

    std::optional<std::string> `A71ChGetEccSignature`(uint8\_t *key\_idx*, *const* std::vector<uint8\_t> &*data*)[Â¶](#_CPPv4N10coralmicro20A71ChGetEccSignatureE7uint8_tRKNSt6vectorI7uint8_tEE "Permalink to this definition")
    :   Gets the Elliptical Curve signature for a hash.

        Parameters
        :   * **key\_idx** â The key storage index of the public key to sign this hash.
            * **sha** â The sha256 hash for some data to sign.

        Returns
        :   The Ecc Signature or std::nullopt on failure.

    std::optional<std::string> `A71ChGetEccSignature`(uint8\_t *key\_idx*, *const* std::string &*sha*)[Â¶](#_CPPv4N10coralmicro20A71ChGetEccSignatureE7uint8_tRKNSt6stringE "Permalink to this definition")
    :   Gets the Elliptical Curve signature for a hash.

        Parameters
        :   * **key\_idx** â The key storage index of the public key to sign this hash.
            * **sha** â The string of sha256 hash for some data to sign.

        Returns
        :   The Ecc Signature or std::nullopt on failure.

    bool `A71ChEccVerify`(uint8\_t *key\_idx*, *const* uint8\_t \**sha*, uint16\_t *sha\_len*, *const* uint8\_t \**signature*, uint16\_t *signature\_len*)[Â¶](#_CPPv4N10coralmicro14A71ChEccVerifyE7uint8_tPK7uint8_t8uint16_tPK7uint8_t8uint16_t "Permalink to this definition")
    :   Verify whether signature is the signature of sha using ecc\_key.

        Parameters
        :   * **key\_idx** â The storage index of the ecc public key that was used to sign the sha.
            * **sha** â The sha256 hash to verify.
            * **sha\_len** â The length of sha.
            * **signature** â The signature to verify.
            * **signature\_len** â The length of the signature.

        Returns
        :   true if verify success, else false.

    bool `A71ChEccVerify`(uint8\_t *key\_idx*, *const* std::string &*sha*, *const* std::string &*signature*)[Â¶](#_CPPv4N10coralmicro14A71ChEccVerifyE7uint8_tRKNSt6stringERKNSt6stringE "Permalink to this definition")
    :   Verify whether signature is the signature of sha using ecc\_key.

        Parameters
        :   * **key\_idx** â The storage index of the ecc public key that was used to sign the sha.
            * **sha** â The sha256 hash to verify.
            * **signature** â The signature to verify.

        Returns
        :   true if verify success, else false.

    bool `A71ChEccVerifyWithKey`(*const* uint8\_t \**ecc\_pub\_key*, uint16\_t *key\_len*, *const* uint8\_t \**sha*, uint16\_t *sha\_len*, *const* uint8\_t \**signature*, uint16\_t *signature\_len*)[Â¶](#_CPPv4N10coralmicro21A71ChEccVerifyWithKeyEPK7uint8_t8uint16_tPK7uint8_t8uint16_tPK7uint8_t8uint16_t "Permalink to this definition")
    :   Verify whether signature is the signature of sha using ecc\_key.

        Parameters
        :   * **ecc\_pub\_key** â The Elliptical Curve public key to use in order to verify. Note: This must be the publickey from the same storage index that was used to sign the sha.
            * **key\_len** â The length of ecc\_pub\_key.
            * **sha** â The sha256 hash to verify.
            * **sha\_len** â The length of sha.
            * **signature** â The signature to verify.
            * **signature\_len** â The length of the signature.

        Returns
        :   true if verify success, else false.

    bool `A71ChEccVerifyWithKey`(*const* std::string &*ecc\_pub\_key*, *const* std::string &*sha*, *const* std::string &*signature*)[Â¶](#_CPPv4N10coralmicro21A71ChEccVerifyWithKeyERKNSt6stringERKNSt6stringERKNSt6stringE "Permalink to this definition")
    :   Verify whether signature is the signature of sha using ecc\_key.

        Parameters
        :   * **ecc\_pub\_key** â The Elliptical Curve public key to use in order to verify. Note: This must be the publickey from the same storage index that was used to sign the sha.
            * **sha** â The sha256 hash to verify.
            * **signature** â The signature to verify.

        Returns
        :   true if verify success, else false.