#!/bin/bash

# Video Frame Interpolation - Azure Deployment Setup Script
# For Mac/Linux users

set -e

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Default values
RESOURCE_GROUP="video-interpolation-rg"
APP_NAME="video-interpolation-app"
REGION="eastus"

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -rg|--resource-group) RESOURCE_GROUP="$2"; shift 2;;
        -an|--app-name) APP_NAME="$2"; shift 2;;
        -r|--region) REGION="$2"; shift 2;;
        *) echo "Unknown option: $1"; exit 1;;
    esac
done

echo -e "${CYAN}========================================${NC}"
echo -e "${CYAN}Video Frame Interpolation - Azure Deployment Setup${NC}"
echo -e "${CYAN}========================================${NC}"

# Check Azure CLI
echo -e "\n${YELLOW}[1/8] Checking Azure CLI...${NC}"
if ! command -v az &> /dev/null; then
    echo -e "${RED}ERROR: Azure CLI not found. Please install it from https://aka.ms/azure-cli${NC}"
    exit 1
fi
echo -e "${GREEN}✓ Azure CLI found${NC}"

# Check if logged in
echo -e "\n${YELLOW}[2/8] Checking Azure login...${NC}"
if ! az account show &> /dev/null; then
    echo -e "${YELLOW}Not logged in. Starting login...${NC}"
    az login
fi
ACCOUNT=$(az account show --query "user.name" -o tsv)
echo -e "${GREEN}✓ Logged in as: $ACCOUNT${NC}"

# Create Resource Group
echo -e "\n${YELLOW}[3/8] Creating Resource Group: $RESOURCE_GROUP...${NC}"
if ! az group show --name "$RESOURCE_GROUP" &> /dev/null; then
    az group create --name "$RESOURCE_GROUP" --location "$REGION"
    echo -e "${GREEN}✓ Resource Group created${NC}"
else
    echo -e "${GREEN}✓ Resource Group already exists${NC}"
fi

# Create App Service Plan
echo -e "\n${YELLOW}[4/8] Creating App Service Plan...${NC}"
PLAN_NAME="$APP_NAME-plan"
if ! az appservice plan show --name "$PLAN_NAME" --resource-group "$RESOURCE_GROUP" &> /dev/null; then
    az appservice plan create \
        --name "$PLAN_NAME" \
        --resource-group "$RESOURCE_GROUP" \
        --sku B1 \
        --is-linux
    echo -e "${GREEN}✓ App Service Plan created (B1 tier)${NC}"
    echo -e "${CYAN}  Note: B1 is for testing. Use B2 or higher for production.${NC}"
else
    echo -e "${GREEN}✓ App Service Plan already exists${NC}"
fi

# Create App Service
echo -e "\n${YELLOW}[5/8] Creating App Service: $APP_NAME...${NC}"
if ! az webapp show --name "$APP_NAME" --resource-group "$RESOURCE_GROUP" &> /dev/null; then
    az webapp create \
        --resource-group "$RESOURCE_GROUP" \
        --plan "$PLAN_NAME" \
        --name "$APP_NAME" \
        --runtime "NODE|18-lts"
    echo -e "${GREEN}✓ App Service created${NC}"
else
    echo -e "${GREEN}✓ App Service already exists${NC}"
fi

# Configure App Settings
echo -e "\n${YELLOW}[6/8] Configuring App Settings...${NC}"
az webapp config appsettings set \
    --resource-group "$RESOURCE_GROUP" \
    --name "$APP_NAME" \
    --settings \
        PORT=3000 \
        NODE_ENV=production \
        WEBSITE_NODE_DEFAULT_VERSION=18.13.0 \
        PYTHON_VERSION=3.9 \
        ENABLE_ORYX_BUILD=true
echo -e "${GREEN}✓ App Settings configured${NC}"

# Get Publish Profile
echo -e "\n${YELLOW}[7/8] Getting Publish Profile...${NC}"
az webapp deployment list-publishing-profiles \
    --resource-group "$RESOURCE_GROUP" \
    --name "$APP_NAME" \
    --xml > PublishProfile.xml
echo -e "${GREEN}✓ Publish Profile saved to PublishProfile.xml${NC}"

# Display next steps
echo -e "\n${YELLOW}[8/8] Setup Complete! Next Steps:${NC}"
echo ""
echo -e "${CYAN}1. Add GitHub Secrets:${NC}"
echo -e "${CYAN}   a) Go to your GitHub repository${NC}"
echo -e "${CYAN}   b) Settings → Secrets and variables → Actions${NC}"
echo -e "${CYAN}   c) Create two secrets:${NC}"
echo -e "${CYAN}      - AZURE_APP_NAME = $APP_NAME${NC}"
echo -e "${CYAN}      - AZURE_PUBLISH_PROFILE = (content of PublishProfile.xml)${NC}"
echo ""
echo -e "${CYAN}2. Push to GitHub:${NC}"
echo -e "   git add . && git commit -m 'Add Azure deployment config' && git push origin main"
echo ""
echo -e "${CYAN}3. Monitor Deployment:${NC}"
echo -e "   Go to GitHub → Actions tab to watch the deployment"
echo ""
echo -e "${CYAN}4. Access Your App:${NC}"
echo -e "   ${GREEN}https://$APP_NAME.azurewebsites.net${NC}"
echo ""
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}✓ Azure setup is ready!${NC}"
echo -e "${GREEN}========================================${NC}"
