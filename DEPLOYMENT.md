# Azure Deployment Guide

## Prerequisites

Before deploying to Azure, ensure you have:

1. **Azure Account**: [Sign up for free](https://azure.microsoft.com/free/)
2. **Azure CLI**: [Install Azure CLI](https://learn.microsoft.com/cli/azure/install-azure-cli)
3. **GitHub Account**: For CI/CD pipeline
4. **Git**: Version control
5. **Python 3.9+**: For video processing
6. **Node.js 18+**: For web server

## Step-by-Step Deployment

### Step 1: Prepare Your Local Repository

```bash
cd "d:\Projects\Video Frame Interpolation\Video Frame Interpolation"
git init
git add .
git commit -m "Initial commit: Video Frame Interpolation"
git remote add origin https://github.com/YOUR_USERNAME/Video-Frame-Interpolation.git
git branch -M main
git push -u origin main
```

### Step 2: Create Azure Resources

#### Option A: Using Azure CLI (Recommended)

```bash
# Login to Azure
az login

# Set variables
$RESOURCE_GROUP="video-interpolation-rg"
$APP_NAME="video-interpolation-app"
$REGION="eastus"

# Create Resource Group
az group create `
  --name $RESOURCE_GROUP `
  --location $REGION

# Create App Service Plan (Free tier for testing)
az appservice plan create `
  --name "video-interpolation-plan" `
  --resource-group $RESOURCE_GROUP `
  --sku B1 `
  --is-linux

# Create App Service
az webapp create `
  --resource-group $RESOURCE_GROUP `
  --plan "video-interpolation-plan" `
  --name $APP_NAME `
  --runtime "NODE|18-lts"

# Configure Python support
az webapp config appsettings set `
  --resource-group $RESOURCE_GROUP `
  --name $APP_NAME `
  --settings `
    PYTHON_VERSION=3.9 `
    NODE_ENV=production `
    PORT=3000
```

#### Option B: Using Azure Portal

1. Go to [Azure Portal](https://portal.azure.com)
2. Click "+ Create a resource"
3. Search for "App Service"
4. Click "Create"
5. Configure:
   - **Resource Group**: Create new - `video-interpolation-rg`
   - **Name**: `video-interpolation-app` (must be globally unique)
   - **Publish**: Code
   - **Runtime Stack**: Node 18 LTS
   - **OS**: Linux
   - **Region**: East US (or nearest to you)
   - **App Service Plan**: Create new (B1 or B2 for testing)
6. Click "Review + Create" → "Create"

### Step 3: Configure Continuous Deployment

#### Get Your Publish Profile

```bash
# Using Azure CLI
az webapp deployment list-publishing-profiles `
  --resource-group $RESOURCE_GROUP `
  --name $APP_NAME `
  --query "[0]" --output json > PublishSettings.json
```

Or from Azure Portal:
1. Go to your App Service
2. Click "Download publish profile"
3. Save the XML file

#### Add to GitHub Secrets

1. Go to your GitHub repository
2. Settings → Secrets and variables → Actions
3. Click "New repository secret"
4. Add two secrets:
   - **Name**: `AZURE_PUBLISH_PROFILE`
   - **Value**: (Paste entire content of your publish profile XML)
   - **Name**: `AZURE_APP_NAME`
   - **Value**: `video-interpolation-app` (your app name)

### Step 4: Enable SSH Access for Debugging

```bash
# Enable SSH on your App Service
az webapp create-remote-connection `
  --resource-group $RESOURCE_GROUP `
  --name $APP_NAME

# Or use Kudu console (built-in browser SSH)
# https://YOUR_APP_NAME.scm.azurewebsites.net/
```

### Step 5: Configure Application Settings

```bash
az webapp config appsettings set `
  --resource-group $RESOURCE_GROUP `
  --name $APP_NAME `
  --settings `
    PORT=3000 `
    NODE_ENV=production `
    WEBSITE_NODE_DEFAULT_VERSION=18.13.0 `
    PYTHON_VERSION=3.9 `
    ENABLE_ORYX_BUILD=true
```

### Step 6: Set Up File Storage

Azure App Service has ephemeral storage - files are deleted when the app restarts.

**For Development**: Use local temp storage (current setup)
**For Production**: Use Azure Blob Storage

```bash
# Create Storage Account
az storage account create `
  --resource-group $RESOURCE_GROUP `
  --name "videointerpstg$(Get-Random)" `
  --location $REGION `
  --sku Standard_LRS

# Create Blob Container
az storage container create `
  --account-name YOUR_STORAGE_ACCOUNT `
  --name videos

# Get Connection String
az storage account show-connection-string `
  --resource-group $RESOURCE_GROUP `
  --name YOUR_STORAGE_ACCOUNT `
  --query connectionString
```

Add connection string to App Service:
```bash
az webapp config connection-string set `
  --resource-group $RESOURCE_GROUP `
  --name $APP_NAME `
  --connection-string-type Custom `
  --settings `
    AZURE_STORAGE_CONNECTION_STRING="YOUR_CONNECTION_STRING"
```

### Step 7: Deploy Your Application

Push to GitHub and the workflow will automatically deploy:

```bash
git add .
git commit -m "Add Azure deployment configuration"
git push origin main
```

Monitor deployment:
1. Go to GitHub repository → Actions
2. Watch the workflow run
3. Check deployment logs

### Step 8: Verify Deployment

```bash
# Get your app URL
az webapp show `
  --resource-group $RESOURCE_GROUP `
  --name $APP_NAME `
  --query "defaultHostName"

# Visit: https://YOUR_APP_NAME.azurewebsites.net
```

### Step 9: Set Up Custom Domain (Optional)

```bash
# Add custom domain
az webapp config hostname add `
  --resource-group $RESOURCE_GROUP `
  --webapp-name $APP_NAME `
  --hostname yourdomain.com
```

### Step 10: Enable HTTPS/SSL

```bash
# Create App Service Managed Certificate
az webapp config hostname add `
  --resource-group $RESOURCE_GROUP `
  --webapp-name $APP_NAME `
  --hostname yourdomain.com

# Bind certificate
az webapp config ssl bind `
  --resource-group $RESOURCE_GROUP `
  --name $APP_NAME `
  --certificate-thumbprint YOUR_THUMBPRINT `
  --ssl-type SNI
```

## Monitoring & Troubleshooting

### View Live Logs

```bash
az webapp log tail `
  --resource-group $RESOURCE_GROUP `
  --name $APP_NAME

# Or use Azure Portal → Diagnose and solve problems
```

### Common Issues

**1. Python not found**
- Ensure `requirements.txt` is in the root directory
- Platform will auto-detect and install Python

**2. Video processing timeout**
- Increase App Service plan tier (B2 or higher)
- Increase `timeout` in server.js if needed

**3. Disk space issues**
- Use Azure Blob Storage for video files
- Implement file cleanup strategy

**4. Large video uploads fail**
- Check `maxRequestLength` in web.config
- Increase App Service plan resources

## Next Steps

1. ✅ Deployment complete!
2. Monitor application performance
3. Set up Application Insights for monitoring
4. Configure auto-scaling based on load
5. Set up continuous monitoring and alerts

## Useful Commands

```bash
# View app status
az webapp show --resource-group $RESOURCE_GROUP --name $APP_NAME

# Restart app
az webapp restart --resource-group $RESOURCE_GROUP --name $APP_NAME

# View configuration
az webapp config show --resource-group $RESOURCE_GROUP --name $APP_NAME

# Delete resources (cleanup)
az group delete --name $RESOURCE_GROUP --yes --no-wait
```

## Support

- [Azure Documentation](https://docs.microsoft.com/azure/)
- [Azure CLI Reference](https://learn.microsoft.com/cli/azure/)
- [App Service Troubleshooting](https://learn.microsoft.com/azure/app-service/troubleshoot-common-app-service-errors)
