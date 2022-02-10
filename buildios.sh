#!/bin/bash

ScriptDir=$(cd $(dirname "$0") && pwd)
BuildDir="$ScriptDir/Build"

rm -rf "$BuildDir"
mkdir -p "$BuildDir"
cd "$BuildDir"

