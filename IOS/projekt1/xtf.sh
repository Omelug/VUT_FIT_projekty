#!/bin/bash
# xjanecv00
# 18.2.2024 - 26.2.2024

n=1

#deafult values
POSIXLY_CORRECT=yes

prikaz=list
uzivatel=""
files=""
currency=""
zcat_cmd=""
after=""
before=""
prikaz=""

date_regex='\d{4}\-(0\d|1[012])\-([012]\d|3[01]) ([01]\d|2[0-3])(\:[0-5]\d){2}'

date_control(){
  if [[ $(echo "$2" | grep -oP "^$date_regex$" | head -1) = "" ]]; then
    echo "$1 value \"$2\" is not date" 1>$2
    exit 145
  fi
}

currency_control(){
  if [[ $(echo "$2" | grep -oP '^( .{3})+$' | head -1) = "" ]]; then
    echo "$1 value \"$2\" is not currency" 1>$2
    exit 2
  fi
}



while [[ $n -le $# ]]
do
    case ${!n} in
      -h|--help)
        printf "xtf [-h|--help] [FILTR] [PŘÍKAZ] UŽIVATEL LOG [LOG2 [...]\n"
        printf "
-h|--help vypisuje tenhle vypis, pak ukončí program

FILTR - filtry pro LOGy
  -a YYYY-MM-DD HH:MM:SS - after
  -b YYYY-MM-DD HH:MM:SS - before
  -c <currency, 3 chars> -currency

PŘÍKAZ - formát výpisu
  list – záznamy uživatele
  list-currency – seřazené měny uživatele
  status – tajny vypis //spouštěte jen pokud jste administrátor
  profit – stonks  ((-_-)/^ 🗠🗠🗠🗠🗠🗠🗠🗠
  ////////////////////////////////////////////
  /                    /\                    /
  /          IIII     /  \                   /
  /         CIOIO    /    \      STONKS!     /
  /          IIII     |  |                   /
  /          II       |  |                   /
  /       TTTTTTTT    |  |                   /
  ////////////////////////////////////////////
UŽIVATEL - jmeno uživatele, nutno uvést

LOG - soubory ve formátu:
  JMENO UZIVATELE;DATUM A CAS;MENA;HODNOTA
  Pokud mají soubory příponu .gz

"
        exit 0 ;;
      list|list-currency|status|profit)
        if [[ $prikaz ]]; then
            echo "Sorry, error PŘÍKAZ PŘÍKAZ" 1>&2
            exit 75
        fi
        prikaz=${!n} ;;
      -a)
        ((n++))
        if [[ $after ]]; then
            echo "Sorry, error after after" 1>&2
            exit 73
        fi
        after=${!n}
        date_control "-a" "$after";;
      -b)
        ((n++))
        if [[ $before ]]; then
            echo "Sorry, error before before" 1>&2
            exit 72
        fi
        before=${!n}
        date_control "-b" "$before"
        ;;
      -c)
        ((n++))
        currency="$currency ${!n}"
        currency_control "-c" "$currency" ;;
      *)
        if [ "$uzivatel" = "" ]; then
          uzivatel=${!n}
        else
          if [[ -z $files ]]; then
            files="${!n}"
          else
            files="$files:${!n}"
          fi
        fi
        ;;
    esac
    ((n++))
done

#input control
if [[ $uzivatel = "" ]]; then
    echo "UZIVATEL not set" 1>&2
    exit 37
fi
if [[ $(echo "$uzivatel" | grep -oP ' '  | wc -l) != 0 ]]; then
    echo "UZIVATEL cant have blank chars" 1>&2
    exit 7
fi

#echo "after=$after"
#echo "before=$before"
#echo "currency=$currency"
#echo "PRIKAZ=$prikaz"
#echo "uzivatel=$uzivatel"
#echo "FILES=$files"

IFS=":"

for file in $files
do
  #echo "HERE $file "
  if [ ! -f "$file" ]; then
     echo "File ""${file}"" was not found" 1>&2
     exit 45
     #files=$(echo "$files" | sed "s~[[:space:]]*${file}[[:space:]]*~~")
  fi

  if [[ $file =~ ^.*\.gz$ ]]; then
    if [[ $zcat_cmd = "" ]]; then
        zcat_cmd="zcat"
    fi
    zcat_cmd="$zcat_cmd \"${file}\""
    files=$(echo "$files" | sed "s~[[:space:]]*${file}[[:space:]]*~~") # ~ used here because file path contains slashes
  fi
  if [[ -n $zcat_cmd ]]; then
        zcat_cmd="$zcat_cmd | "
        echo $zcat_cmd
  fi
done

#echo "Reduced files $files $zcat_cmd"

if [[ "$files" = "" ]] && [[ "$zcat_cmd" = "" ]]; then
  echo "Script needs valid log files" 1>&2
  exit 4
fi

IFS=":"
for file in $files
do
  files2="$files2 \"${file}\""
done

prefilters="$zcat_cmd awk -F ';' ' {print \$0}' $files2 - | "


if [[ -z $zcat_cmd ]]; then
    prefilters="awk -F ';' '{print \$0}' $files2 |"
fi

#eval "$prefilters | cat - "
#echo "$(eval "$prefilters grep -P \"^(?!.+?;$date_regex;.{3};-?\d+(?:.\d+))\" - ")"
#exit 5

if [[ -n $(eval "$prefilters grep -P \"^(?!.+?;$date_regex;.{3};-?\d+(?:.\d+))\" - ") ]]; then
  echo "Format is invalid" 1>&2
  exit 52
fi

if [[ -n $after ]]; then
    prefilters="$prefilters awk -F ';' -v after=\"$after\" '{if(after<\$2){print \$0}}' - | "
fi

if [[ -n $before ]]; then
    prefilters="$prefilters awk -F ';' -v before=\"$before\" '{if(before>\$2){print \$0}}' - | "
fi

if [[ -n $currency ]]; then
  prefilters="$prefilters awk -F ';' -v c=\"$currency\" 'c ~ \$3 {print \$0}' - | "
fi

cmd="$prefilters awk -F ';' -v uzivatel=\"$uzivatel\" '\$1 == uzivatel {print \$0}' - "

case $prikaz in
  "list-currency")
    cmd="$cmd | awk -F ';' '{print \$3}' | sort -u"
  ;;
  "status")
    cmd="$cmd | awk -F ';' '{sum[\$3]+=\$4} END{for(cur in sum){printf(\"%s : %.4f\n\",cur,sum[cur])}}' - | sort "
  ;;
  "profit")
    nasobek=120

    #echo $XTF_PROFIT
    if [[ -n $XTF_PROFIT ]]; then
        nasobek=$((100 + ${XTF_PROFIT}))
    fi
    cmd="$prefilters awk -F ';' -v uzivatel=\"$uzivatel\" -v nasobek=$nasobek '\$1 == uzivatel {sum[\$3]+=\$4} END{for(cur in sum){printf(\"%s : %.4f\n\",cur,sum[cur]*(sum[cur]<0?1:nasobek/100))}}' - | sort "
  ;;
esac

echo "$cmd" 1>&2

eval "$cmd"
#echo $? 1>&2