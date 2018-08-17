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

  echo "-----------------------\n";
  echo "Generate public from private..\n";
  $public_key_new = "";
  mhcrypto_generate_public($private_key, $public_key_new);
  if($public_key == $public_key_new)
    echo "Public key match\n";
  else
    echo "Public key does not match\n";

  echo "Generate address from public..\n";
  $mh_address_new = "";
  mhcrypto_generate_address($public_key_new, $mh_address_new);
  if($mh_address == $mh_address_new)
    echo "Address match\n";
  else
    echo "Address does not match\n";

  echo "Verifying address..\n";
  if(mhcrypto_check_address($mh_address_new))
    echo "Address good\n";
  else
    echo "Address bad\n";
?>
