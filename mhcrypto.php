<?php
  if(!extension_loaded('mhcrypto')) {
    dl('mhcrypto.' . PHP_SHLIB_SUFFIX);
  }

  $private_key = "";
  $public_key = "";
  $mh_address = "";
  mhcrypto_generate_wallet($private_key, $public_key, $mh_address);

  echo "Generated private key: " . $private_key . "\n";
  echo "Generated public key: " . $public_key . "\n";
  echo "Generated wallet address: " . $mh_address . "\n";
?>
