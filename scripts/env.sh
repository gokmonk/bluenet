#!/bin/bash

# assign default BLUENET_DIR environment variable to the parent folder of the folder containing this script
if [ -z "${BLUENET_DIR}" ]; then
	export BLUENET_DIR=$(readlink -m $( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/..)
fi

# load environment variables from env.config (if it exists)
if [ -e "${BLUENET_DIR}/env.config" ]; then
	set -o allexport
	source ${BLUENET_DIR}/env.config
	set +o allexport
fi

# assign default paths to the environment variables if not defined otherwise
if [ ! -z "${BLUENET_WORKSPACE_DIR}" ]; then
	if [ -z "${BLUENET_DIR}" ]; then
		export BLUENET_DIR=${BLUENET_WORKSPACE_DIR}/bluenet
	fi
	if [ -z "${BLUENET_CONFIG_DIR}" ]; then
		export BLUENET_CONFIG_DIR=${BLUENET_WORKSPACE_DIR}/config
	fi
	if [ -z "${BLUENET_BUILD_DIR}" ]; then
		export BLUENET_BUILD_DIR=${BLUENET_WORKSPACE_DIR}/build
	fi
	if [ -z "${BLUENET_BIN_DIR}" ]; then
		export BLUENET_BIN_DIR=${BLUENET_WORKSPACE_DIR}/bin
	fi
	if [ -z "${BLUENET_RELEASE_DIR}" ]; then
		export BLUENET_RELEASE_DIR=${BLUENET_WORKSPACE_DIR}/release
	fi
	if [ -z "${BLUENET_BOOTLOADER_DIR}" ]; then
		export BLUENET_BOOTLOADER_DIR=${BLUENET_WORKSPACE_DIR}/bootloader
	fi
fi
