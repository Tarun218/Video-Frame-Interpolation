#!/usr/bin/env pwsh

<#
.SYNOPSIS
    Quick deployment script for Video Frame Interpolation to Azure App Service
    
.DESCRIPTION
    Automates the setup of Azure resources and deployment configuration
    
.PARAMETER ResourceGroup
    Azure Resource Group name (default: video-interpolation-rg)
    
.PARAMETER AppName
    Application name for Azure App Service (default: video-interpolation-app)
    
.PARAMETER Region
    Azure region for deployment (default: eastus)
#>

param(
    [string]$ResourceGroup = "video-interpolation-rg",
    [string]$AppName = "video-interpolation-app",
    [string]$Region = "eastus"
)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Video Frame Interpolation - Azure Deployment Setup" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

# Check Azure CLI
Write-Host "`n[1/8] Checking Azure CLI..." -ForegroundColor Yellow
if (-not (Get-Command az -ErrorAction SilentlyContinue)) {
    Write-Host "ERROR: Azure CLI not found. Please install it from https://aka.ms/azure-cli" -ForegroundColor Red
    exit 1
}
Write-Host "✓ Azure CLI found" -ForegroundColor Green

# Check if logged in
Write-Host "`n[2/8] Checking Azure login..." -ForegroundColor Yellow
$account = az account show -o json 2>$null | ConvertFrom-Json
if ($null -eq $account) {
    Write-Host "Not logged in. Starting login..." -ForegroundColor Yellow
    az login
    $account = az account show -o json | ConvertFrom-Json
}
Write-Host "✓ Logged in as: $($account.user.name)" -ForegroundColor Green

# Create Resource Group
Write-Host "`n[3/8] Creating Resource Group: $ResourceGroup..." -ForegroundColor Yellow
$rgExists = az group exists --name $ResourceGroup
if ($rgExists -eq "false") {
    az group create --name $ResourceGroup --location $Region
    Write-Host "✓ Resource Group created" -ForegroundColor Green
} else {
    Write-Host "✓ Resource Group already exists" -ForegroundColor Green
}

# Create App Service Plan
Write-Host "`n[4/8] Creating App Service Plan..." -ForegroundColor Yellow
$planName = "$AppName-plan"
$planExists = az appservice plan show --name $planName --resource-group $ResourceGroup -o json 2>$null
if ($null -eq $planExists) {
    az appservice plan create `
        --name $planName `
        --resource-group $ResourceGroup `
        --sku B1 `
        --is-linux
    Write-Host "✓ App Service Plan created (B1 tier)" -ForegroundColor Green
    Write-Host "  Note: B1 is for testing. Use B2 or higher for production." -ForegroundColor Gray
} else {
    Write-Host "✓ App Service Plan already exists" -ForegroundColor Green
}

# Create App Service
Write-Host "`n[5/8] Creating App Service: $AppName..." -ForegroundColor Yellow
$appExists = az webapp show --name $AppName --resource-group $ResourceGroup -o json 2>$null
if ($null -eq $appExists) {
    az webapp create `
        --resource-group $ResourceGroup `
        --plan $planName `
        --name $AppName `
        --runtime "NODE|18-lts"
    Write-Host "✓ App Service created" -ForegroundColor Green
} else {
    Write-Host "✓ App Service already exists" -ForegroundColor Green
}

# Configure App Settings
Write-Host "`n[6/8] Configuring App Settings..." -ForegroundColor Yellow
az webapp config appsettings set `
    --resource-group $ResourceGroup `
    --name $AppName `
    --settings `
        PORT=3000 `
        NODE_ENV=production `
        WEBSITE_NODE_DEFAULT_VERSION=18.13.0 `
        PYTHON_VERSION=3.9 `
        ENABLE_ORYX_BUILD=true
Write-Host "✓ App Settings configured" -ForegroundColor Green

# Get Publish Profile
Write-Host "`n[7/8] Getting Publish Profile..." -ForegroundColor Yellow
$publishProfile = az webapp deployment list-publishing-profiles `
    --resource-group $ResourceGroup `
    --name $AppName `
    --xml
$publishProfile | Out-File -FilePath "PublishProfile.xml" -Force
Write-Host "✓ Publish Profile saved to PublishProfile.xml" -ForegroundColor Green

# Display next steps
Write-Host "`n[8/8] Setup Complete! Next Steps:" -ForegroundColor Yellow
Write-Host ""
Write-Host "1. Add GitHub Secrets:" -ForegroundColor Cyan
Write-Host "   a) Go to your GitHub repository" -ForegroundColor Gray
Write-Host "   b) Settings → Secrets and variables → Actions" -ForegroundColor Gray
Write-Host "   c) Create two secrets:" -ForegroundColor Gray
Write-Host "      - AZURE_APP_NAME = $AppName" -ForegroundColor Gray
Write-Host "      - AZURE_PUBLISH_PROFILE = (content of PublishProfile.xml)" -ForegroundColor Gray
Write-Host ""
Write-Host "2. Push to GitHub:" -ForegroundColor Cyan
Write-Host "   git add ." -ForegroundColor Gray
Write-Host "   git commit -m 'Add Azure deployment config'" -ForegroundColor Gray
Write-Host "   git push origin main" -ForegroundColor Gray
Write-Host ""
Write-Host "3. Monitor Deployment:" -ForegroundColor Cyan
Write-Host "   Go to GitHub → Actions tab to watch the deployment" -ForegroundColor Gray
Write-Host ""
Write-Host "4. Access Your App:" -ForegroundColor Cyan
Write-Host "   https://$AppName.azurewebsites.net" -ForegroundColor Cyan
Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "✓ Azure setup is ready!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
