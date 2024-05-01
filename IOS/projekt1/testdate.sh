#!/bin/bash
echo "$XTF_PROFIT"
exit 5
if [[ $(echo "$1" | grep -oP '^\d{4}\-(0\d|1[012])\-([012]\d|3[01]) ([01]\d|2[0-4])(:([0-5]\d|60)){2}$' | head -1) = "" ]]; then
  echo "$1 is not date"
  exit 0
fi
echo OK