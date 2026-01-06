# PowerShell script to run GUI application with GTK3 DLLs
# This script adds MSYS2 MinGW64 GTK3 DLLs to PATH and runs the application

# MSYS2 MinGW64 bin directory (adjust if your MSYS2 is installed elsewhere)
$msys2Path = "C:\msys64\mingw64\bin"

# Check if MSYS2 path exists
if (-not (Test-Path $msys2Path)) {
    Write-Host "Hata: MSYS2 MinGW64 bulunamadi: $msys2Path" -ForegroundColor Red
    Write-Host "Lutfen MSYS2'nin kurulu oldugu yolu kontrol edin." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Alternatif olarak, MSYS2 MinGW64 terminalinde calistirin:" -ForegroundColor Yellow
    Write-Host "  cd /c/ders/C/randevu" -ForegroundColor Cyan
    Write-Host "  ./randevu_sistemi.exe --gui" -ForegroundColor Cyan
    exit 1
}

# Add MSYS2 bin to PATH for this session
$env:PATH = "$msys2Path;$env:PATH"

# Check if GTK DLL exists
$gtkDll = Join-Path $msys2Path "libgtk-3-0.dll"
if (-not (Test-Path $gtkDll)) {
    Write-Host "Uyari: GTK3 DLL bulunamadi: $gtkDll" -ForegroundColor Yellow
    Write-Host "GTK3 kurulu mu kontrol edin:" -ForegroundColor Yellow
    Write-Host "  pacman -S mingw-w64-x86_64-gtk3" -ForegroundColor Cyan
    Write-Host ""
}

# Change to script directory
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $scriptDir

# Check if executable exists
if (-not (Test-Path "randevu_sistemi.exe")) {
    Write-Host "Hata: randevu_sistemi.exe bulunamadi!" -ForegroundColor Red
    Write-Host "Lutfen once projeyi derleyin (MSYS2 MinGW64 terminalinde)." -ForegroundColor Yellow
    exit 1
}

# Run the application with --gui flag
Write-Host "GUI modu baslatiliyor..." -ForegroundColor Green
Write-Host ""
& ".\randevu_sistemi.exe" --gui

