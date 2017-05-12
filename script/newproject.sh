#!/usr/bin/env bash

read -p 'project name:' PROJ_NAME

if [[ ${PROJ_NAME}x == ''x ]]; then
    echo "invalid project name.'$PROJ_NAME'"
    exit
fi

MetaConfigName=meta.config
declare -A MetaConfig

MetaConfig["Project"]=${PROJ_NAME}
MetaConfig["Solution"]=${PROJECT_NAME}
MetaConfig["DateTime"]=$(date "+%Y-%m-%d")


function readConfigLine() {
  local line=$1
  local key=${line%%=*}
  local def=${line##*=}
  if [[ "$key"x != ''x ]]; then
	  read -p ${filename}': '$key'('$def')' val < /dev/tty
	  if [[ "$val"x != ''x ]]; then
		  def=$val
	  fi
	  MetaConfig[$key]=$def
	  echo ${MetaConfig[$key]}
  fi
}

function metaCopy () {
	local filename=$1
	echo ${filename}
	if [[ ${MetaConfigName} == ${filename##*/} ]]; then
		while read line
		do
			readConfigLine ${line}
		done < ${filename}
		readConfigLine ${line}
	elif [[ -d ${filename} ]]; then
		if [[ ! -d $2 ]]; then
			mkdir $2
		fi
		if [[ -f ${filename}/${MetaConfigName} ]]; then
		    metaCopy ${filename}/${MetaConfigName} $2/${filename}
		fi
	    for file in $(ls ${filename})
		do
			if [[ ${file} != ${MetaConfigName} ]]; then
			    metaCopy ${filename}/${file} $2/${file}
		    fi
		done
	elif [[ -f ${filename} ]]; then
		# replace
		local file=$(cat $1)
		local key_set=$(echo ${file} | sed -n 's/.*{%[[:space:]]\+\(\w\+\)[[:space:]]\+%}.*/\1/'p)
		while [[ ${key_set}x != ''x ]]
		do
			local key=$(echo ${key_set} | head -n 1)
			local keys=${!MetaConfig[@]}
			if [[ "${keys/$key/}" == "${!MetaConfig[@]}" ]]; then
				read -p 'please input "'$key'":' val
				MetaConfig[$key]=$val
			fi
			file=$(echo "${file}" | sed "s/{% \+$key \+%}/${MetaConfig[$key]}/g")
			local key_set=$(echo ${file} | sed -n 's/.*{%[[:space:]]\+\(\w\+\)[[:space:]]\+%}.*/\1/'p)
		done
		outfile=$2
		outfile=$(echo "${outfile}" | sed "s/{%Solution%}/"${MetaConfig["Solution"]}"/g")
		outfile=$(echo "${outfile}" | sed "s/{%Project%}/"${MetaConfig["Project"]}"/g")
		echo "${filename} -> $outfile"
		echo "${file}" > $outfile
	fi
}

mkdir src/${PROJ_NAME}
metaCopy meta src/${PROJ_NAME}


