Param(
    [Parameter(Mandatory=$false)]
    [Switch]$clean,
    [Parameter(Mandatory=$false)]
    [Switch]$docs
)

# if user specified clean, remove all build files
if ($clean.IsPresent)
{
    if (Test-Path -Path "build")
    {
        remove-item build -R
    }
}

if (($clean.IsPresent) -or (-not (Test-Path -Path "build")))
{
    new-item -Path build -ItemType Directory
}

# Check if ./extern/includes/bs-cordl/version.txt exists
if (Test-Path "$PSScriptRoot/../extern/includes/bs-cordl/include/version.txt") {
    # Update packageVersion in mod.template.json using bs-cordl version.txt
    $modTemplateRaw = Get-Content "$PSScriptRoot/../mod.template.json" -Raw
    $modTemplateOriginal = $modTemplateRaw | ConvertFrom-Json
    $modTemplate = $modTemplateRaw | ConvertFrom-Json
    $bsversion = Get-Content "$PSScriptRoot/../extern/includes/bs-cordl/include/version.txt"
    if (-not [string]::IsNullOrWhitespace($bsversion)) {
        Write-Output "Setting Package Version to $bsversion"
        $modTemplate.packageVersion = $bsversion

        # Write the updated mod.template.json if the contents have changed
        if (($modTemplate | ConvertTo-Json -Depth 32) -ne ($modTemplateOriginal | ConvertTo-Json -Depth 32)) {
            Write-Output "Writing updated mod.template.json"
            $modTemplate | ConvertTo-Json -Depth 32 | Set-Content -Path "$PSScriptRoot/../mod.template.json"
        }
    }
    else {
        Write-Output "Empty bs-cordl version.txt, skipping package version update."
    }
}
else {
    Write-Output "Missing bs-cordl version.txt, skipping package version update."
}

$make_docs = "-DMAKE_DOCS=" + $docs.IsPresent.ToString().ToLower()

& cmake -G "Ninja" -DCMAKE_BUILD_TYPE="RelWithDebInfo" $make_docs . -B build
& cmake --build ./build

exit $LastExitCode
