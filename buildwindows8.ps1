Param( [switch] $Debug, [switch] $EngineOnly )

$ScriptPath   = Split-Path -Parent $MyInvocation.MyCommand.Path

$BuildDir32   = Join-Path $ScriptPath "Build"
$BuildDir64   = Join-Path $ScriptPath "Build64"
$BuildDirARM  = Join-Path $ScriptPath "BuildARM"
$BuildDirWin8 = Join-Path $ScriptPath "TetraSlide.Win8"
$OutputDir    = Join-Path $BuildDirWin8 "Output"

$Version = "1.0.0.1"

$ConfigurationName = "Release"
if ( $Debug )
{
	$ConfigurationName = "Debug"
}

# Assume the standard install path for Visual Studio 2012
Set-Variable -Name MVSNET -value "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise" -scope Global
Set-Variable -Name VCBUILD_CMD -value "msbuild.exe" -scope Global

if ( Test-Path -PathType Container "$BuildDir32" )
{
	rm -Recurse -Force "$BuildDir32"
}
if ( Test-Path -PathType Container "$BuildDir64" )
{
	rm -Recurse -Force "$BuildDir64"
}
if ( Test-Path -PathType Container "$BuildDirARM" )
{
	rm -Recurse -Force "$BuildDirARM"
}
if ( Test-Path -PathType Container "$OutputDir" )
{
	rm -Recurse -Force "$OutputDir"
}

function ChangeDirectoryForReal( $dir )
{
	cd $dir
	[Environment]::CurrentDirectory = Get-Location -PSProvider FileSystem
}

function ModifyEnvironment( [string] $script, [string] $parameters )
{
	"Running batch script $script"
	$tempFile = [IO.Path]::GetTempFileName()

	## Store the output of cmd.exe.  We also ask cmd.exe to output
	## the environment table after the batch file completes
	cmd /c " `"$script`" $parameters && set > `"$tempFile`" "

	## Go through the environment variables in the temp file.
	## For each of them, set the variable in our local environment.
	Get-Content $tempFile | Foreach-Object {
		if ( $_ -match "^(.*?)=(.*)$" )
		{
			Set-Content "env:\$($matches[1])" $matches[2]
		}
	}
	Remove-Item $tempFile
}

function RunCMakeAndNMake( $config, $vars )
{
	Write-Host "---  BEGIN CMake & NMake for $config  ---"
	$VCVARS_BAT = $MVSNET + $vars
	ModifyEnvironment "$VCVARS_BAT"
	Write-Host "Platform: `"$env:Platform`""
	& cmake -D "CMAKE_BUILD_TYPE=$config" -G "NMake Makefiles" $ScriptPath
	& nmake
	Write-Host "---   END  CMake & NMake for $config  ---"
}

function VSBuild( $solution, $config, $platform, $vars )
{
	Write-Host "---  BEGIN $platform / $config  ---"
	$VCVARS_BAT = $MVSNET + $vars
	ModifyEnvironment "$VCVARS_BAT"
	$platform_param = "/p:Platform=" + $platform
	$config_param = "/p:Configuration=" + $config
	& $VCBUILD_CMD $solution /t:Clean $config_param $platform_param
	& $VCBUILD_CMD $solution $config_param $platform_param
	Write-Host "---   END  $platform / $config  ---"
}

function CopyPackageToOutput( $package, $output )
{
	Write-Host "Copying $package to $output"
	if ( -not (Test-Path -PathType Leaf "$package") )
	{
		Write-Error "$package does not exist"
		throw "No Package!"
	}
	if ( -not (Test-Path -PathType Container "$output") )
	{
		Write-Error "$output not a valid target"
		throw "No Output Dir!"
	}
	Copy-Item -Path "$package" -Destination "$output" -Force
}

function UnZipFile( $zipfilename, $destination )
{
	$shellApplication = New-Object -com Shell.Application
	$zipPackage = $shellApplication.NameSpace( $zipfilename )
	$destinationFolder = $shellApplication.NameSpace( $destination )
	# CopyHere vOptions Flag # 4 - Do not display a progress dialog box.
	# 16 - Respond with "Yes to All" for any dialog box that is displayed.
	$destinationFolder.CopyHere( $zipPackage.Items(),20 )
}

function ZipFiles( $zipfilename, $sourcedir )
{
	[Reflection.Assembly]::LoadWithPartialName( "System.IO.Compression.FileSystem" )
	$compressionLevel = [System.IO.Compression.CompressionLevel]::Optimal
	[System.IO.Compression.ZipFile]::CreateFromDirectory( $sourcedir, $zipfilename, $compressionLevel, $false )
}

try
{
	# Build CMake projects using NMake
	mkdir "$BuildDir32"
	ChangeDirectoryForReal "$BuildDir32"
	RunCMakeAndNMake "$ConfigurationName" "\VC\bin\vcvars32.bat"
	ChangeDirectoryForReal "$ScriptPath"

	mkdir "$BuildDir64"
	ChangeDirectoryForReal "$BuildDir64"
	RunCMakeAndNMake "$ConfigurationName" "\VC\bin\amd64\vcvars64.bat"
	ChangeDirectoryForReal "$ScriptPath"

	mkdir "$BuildDirARM"
	ChangeDirectoryForReal "$BuildDirARM"
	RunCMakeAndNMake "$ConfigurationName" "\VC\bin\x86_arm\vcvarsx86_arm.bat"
	ChangeDirectoryForReal "$ScriptPath"

	if ( $EngineOnly )
	{
		Write-Host "Only building engine, exiting..."
		exit
	}

	# Run Windows 8 build
	ChangeDirectoryForReal "$BuildDirWin8"
	VSBuild "TetraSlide.Win8.sln" $ConfigurationName "Win32" "\VC\bin\vcvars32.bat"
	VSBuild "TetraSlide.Win8.sln" $ConfigurationName "x64" "\VC\bin\amd64\vcvars64.bat"
	VSBuild "TetraSlide.Win8.sln" $ConfigurationName "ARM" "\VC\bin\x86_arm\vcvarsx86_arm.bat"
	ChangeDirectoryForReal "$ScriptPath"

	# Gather built packages
	Write-Host "Checking for built packages ..."
	mkdir "$OutputDir"
	ChangeDirectoryForReal "$OutputDir"
	$PackageDir = Join-Path (Join-Path "$BuildDirWin8" "TetraSlide.Win8") "AppPackages"
	$configspec = ""
	if ( $ConfigurationName -ieq "Debug" )
	{
		$configspec = "_Debug"
	}
	CopyPackageToOutput (Join-Path "$PackageDir" "TetraSlide.Win8_${Version}_x86${configspec}.appxupload") "$OutputDir"
	CopyPackageToOutput (Join-Path "$PackageDir" "TetraSlide.Win8_${Version}_x64${configspec}.appxupload") "$OutputDir"
	CopyPackageToOutput (Join-Path "$PackageDir" "TetraSlide.Win8_${Version}_ARM${configspec}.appxupload") "$OutputDir"

	Write-Host "Building side-loadable ZIP file ..."
	Get-ChildItem "$OutputDir" | ForEach-Object {
		Copy-Item -Path "$_" -Destination "${_}.zip" -Force
		UnZipFile (Join-Path "$OutputDir" "${_}.zip") "$OutputDir"
		Remove-Item (Join-Path "$OutputDir" "${_}.zip")
	}
	Get-ChildItem "$OutputDir" -Filter "*.appxsym" | ForEach-Object {
		Remove-Item (Join-Path "$OutputDir" $_)
	}
	if ( $ConfigurationName -ieq "Release" )
	{
		mkdir "Ts"
		Copy-Item -Path (Join-Path "$OutputDir" "TetraSlide.Win8_${Version}_x64${configspec}.appx") -Destination (Join-Path "$OutputDir" "Ts") -Force
		Copy-Item -Path (Join-Path "$BuildDirWin8" "InstallTetraSlide.bat") -Destination (Join-Path "$OutputDir" "Ts") -Force
		ZipFiles (Join-Path "$OutputDir" "TetraSlide.zip") (Join-Path "$OutputDir" "Ts")
		Remove-Item -Path (Join-Path "$OutputDir" "Ts") -Recurse -Force
	}
	ChangeDirectoryForReal "$ScriptPath"
	Write-Host "BUILD SUCCESSFUL!"
	Write-Host "Your packages are located in: $OutputDir"
}
catch
{
	Write-Error "BUILD FAILED!"
	$_ | fl * -Force
}
finally
{
	ChangeDirectoryForReal "$ScriptPath"
}