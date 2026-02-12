# Simple Build Script for Windows
# Mini Online Judge System

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Mini Online Judge - Build Script" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Check if g++ is available
Write-Host "[1/3] Checking for g++ compiler..." -ForegroundColor Yellow
try {
    $null = & g++ --version 2>&1
    if ($LASTEXITCODE -ne 0) { throw }
    Write-Host "✓ g++ found" -ForegroundColor Green
}
catch {
    Write-Host "ERROR: g++ compiler not found!" -ForegroundColor Red
    Write-Host "Please install MinGW or another C++ compiler." -ForegroundColor Red
    exit 1
}
Write-Host ""

# Create necessary directories
Write-Host "[2/3] Creating directories..." -ForegroundColor Yellow
$dirs = @("data", "logs", "submissions", "executables")
foreach ($dir in $dirs) {
    if (!(Test-Path $dir)) {
        New-Item -ItemType Directory -Path $dir -Force | Out-Null
    }
}
Write-Host "✓ Directories ready" -ForegroundColor Green
Write-Host ""

# Compile and link in one step
Write-Host "[3/3] Compiling and linking..." -ForegroundColor Yellow
$sourceFiles = @(
    "src/Utils.cpp",
    "src/User.cpp",
    "src/Problem.cpp",
    "src/Submission.cpp",
    "src/Compiler.cpp",
    "src/JudgeEngine.cpp",
    "src/Leaderboard.cpp",
    "main.cpp"
)

$output = & g++ -std=c++17 -Wall -Wextra -O2 -Iinclude $sourceFiles -o judge.exe 2>&1

if ($LASTEXITCODE -ne 0) {
    Write-Host "✗ Build failed!" -ForegroundColor Red
    if ($output) {
        Write-Host "`nCompilation errors:" -ForegroundColor Red
        Write-Host $output -ForegroundColor Gray
    }
    exit 1
}

Write-Host "✓ Build successful" -ForegroundColor Green
Write-Host ""

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  BUILD COMPLETE!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Run the program with: .\judge.exe" -ForegroundColor Yellow
Write-Host ""
