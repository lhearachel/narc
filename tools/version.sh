#!/usr/bin/env bash

VERSION_FILE=$1
TARGET_FILE=$2

VERSION=$(cat "${VERSION_FILE}")

if test -z "${TARGET_FILE}"; then
    echo "${VERSION}"
else
    {
        echo "/* THIS HEADER IS AUTOMATICALLY GENERATED */"
        echo "/*          DO NOT MODIFY IT!!!           */"
        echo "#ifndef NARC_VERSION_H"
        echo "#define NARC_VERSION_H"
        echo ""
        echo "#define NARC_VERSION \"${VERSION}\""
        echo ""
        echo "#endif /* NARC_VERSION_H */"
    } >"${TARGET_FILE}"
fi
