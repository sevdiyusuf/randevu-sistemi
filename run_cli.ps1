# PowerShell script to run CLI application
# This script runs the application in CLI mode (no GTK DLLs needed)

# Change to script directory
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $scriptDir

# Check if executable exists
if (-not (Test-Path "randevu_sistemi.exe")) {
    Write-Host "Hata: randevu_sistemi.exe bulunamadi!" -ForegroundColor Red
    Write-Host "Lutfen once projeyi derleyin." -ForegroundColor Yellow
    exit 1
}

# Run the application in CLI mode
Write-Host "CLI modu baslatiliyor..." -ForegroundColor Green
Write-Host ""
& ".\randevu_sistemi.exe"

