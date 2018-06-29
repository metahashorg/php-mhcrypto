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

  echo "\n";

  $text = "Hello #Metahash!";
  $sign = "";
  echo "Signing text with private key: " . $text . "\n";
  mhcrypto_sign_text($sign, $private_key, $text);

  $result = "FAIL";
  if(mhcrypto_check_sign_text($sign, $public_key, $text))
    $result = "PASS";
  echo "Check signature with public key: " . $result . "\n";
?>
