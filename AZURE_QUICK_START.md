# 🚀 Quick Start: Deploy to Azure

This guide walks you through deploying **Video Frame Interpolation** to Azure App Service in under 15 minutes.

## ⚡ Super Quick Start (Automated)

### For Windows Users:
```powershell
# 1. Make sure you have Azure CLI installed
# Download from: https://aka.ms/azure-cli

# 2. Run the deployment script
.\deploy-azure.ps1

# 3. Follow the on-screen instructions to add GitHub secrets
```

### For Mac/Linux Users:
```bash
# 1. Install Azure CLI
curl -sL https://aka.ms/InstallAzureCLIDeb | sudo bash

# 2. Make the script executable
chmod +x deploy-azure.sh

# 3. Run it
./deploy-azure.sh
```

---

## 📋 Manual Step-by-Step Deployment

### Prerequisites

Before you start, install these tools:

1. **Azure CLI** - [Download](https://aka.ms/azure-cli)
2. **GitHub Account** - [Sign up free](https://github.com/signup)
3. **Azure Account** - [Free tier available](https://azure.microsoft.com/free/)

Verify installation:
```bash
az --version
git --version
node --version
```

---

### Step 1: Prepare Your GitHub Repository

```bash
# Navigate to your project
cd "d:\Projects\Video Frame Interpolation\Video Frame Interpolation"

# Initialize Git (if not already done)
git init
git add .
git commit -m "Initial commit: Video Frame Interpolation"

# Create repository on GitHub and push
git remote add origin https://github.com/YOUR_USERNAME/Video-Frame-Interpolation.git
git branch -M main
git push -u origin main
```

---

### Step 2: Create Azure Resources (5 minutes)

#### Option A: Automated (Recommended)

**Windows PowerShell:**
```powershell
# Run the automated setup
.\deploy-azure.ps1

# Or customize with your own names
.\deploy-azure.ps1 -ResourceGroup "my-video-rg" `
                   -AppName "my-video-app" `
                   -Region "westus"
```

**Mac/Linux:**
```bash
chmod +x deploy-azure.sh
./deploy-azure.sh
```

#### Option B: Manual Using Azure Portal

1. Go to [Azure Portal](https://portal.azure.com) →  "Create a resource"
2. Search for **App Service**
3. Click **Create**
4. Fill in:
   - **Resource Group**: Create new → `video-interpolation-rg`
   - **Name**: `video-interpolation-app-YOUR-NAME` (must be globally unique)
   - **Publish**: `Code`
   - **Runtime Stack**: `Node 18 LTS`
   - **OS**: `Linux`
   - **Region**: Choose your region
   - **App Service Plan**: Create new → `B1` (testing) or `B2` (production)
5. Click **Create** and wait ~2 minutes

---

### Step 3: Configure GitHub Secrets (3 minutes)

You need to add credentials for automatic deployment.

#### Get Your Publish Profile:

**Using Azure CLI:**
```bash
az login

# Download publish profile
az webapp deployment list-publishing-profiles `
  --resource-group video-interpolation-rg `
  --name video-interpolation-app `
  --xml > PublishProfile.xml

# Display the content
type PublishProfile.xml
```

Or from **Azure Portal**:
1. Go to your App Service
2. Click **"Download publish profile"** (top-right)
3. Save the downloaded file

#### Add GitHub Secrets:

1. Open your GitHub repository
2. Go to **Settings** → **Secrets and variables** → **Actions**
3. Click **"New repository secret"**
4. Create two secrets:

**Secret 1:**
- Name: `AZURE_APP_NAME`
- Value: `video-interpolation-app` (your app name)

**Secret 2:**
- Name: `AZURE_PUBLISH_PROFILE`
- Value: (Entire contents of PublishProfile.xml)

---

### Step 4: Deploy! (2 minutes)

```bash
# Make sure all changes are committed
git add .
git commit -m "Add Azure deployment configuration"

# Push to GitHub (this triggers automatic deployment)
git push origin main
```

**Watch the deployment:**
1. Go to your GitHub repository
2. Click **Actions** tab
3. Watch the workflow run
4. Once complete (green checkmark), your app is deployed!

---

### Step 5: Access Your App

Your app is now live at:
```
https://video-interpolation-app.azurewebsites.net
```

Replace `video-interpolation-app` with your actual app name.

---

## 🔧 Configuration After Deployment

### View Application Logs

```bash
az webapp log tail `
  --resource-group video-interpolation-rg `
  --name video-interpolation-app
```

### Update App Settings

```bash
az webapp config appsettings set `
  --resource-group video-interpolation-rg `
  --name video-interpolation-app `
  --settings `
    NODE_ENV=production `
    MAX_FILE_SIZE=500
```

### Restart the App

```bash
az webapp restart `
  --resource-group video-interpolation-rg `
  --name video-interpolation-app
```

---

## 📊 Monitoring & Troubleshooting

### Check App Status

```bash
az webapp show `
  --resource-group video-interpolation-rg `
  --name video-interpolation-app `
  --query "{name:name, status:state, plan:appServicePlanId}"
```

### Common Issues & Solutions

**Issue: "Python not found"**
- Solution: Add `requirements.txt` to root (already done) ✓

**Issue: "Video processing timeout"**
- Solution: Upgrade App Service plan from B1 to B2
  ```bash
  az appservice plan update `
    --name video-interpolation-plan `
    --resource-group video-interpolation-rg `
    --sku B2
  ```

**Issue: "Large file uploads fail"**
- Solution: Already configured in `web.config` ✓

**Issue: App won't start**
- Solution: Check logs
  ```bash
  az webapp log tail `
    --resource-group video-interpolation-rg `
    --name video-interpolation-app
  ```

---

## 🛡️ Security Recommendations

### Enable HTTPS (Already enabled by default)
✓ Azure provides free SSL certificate

### Restrict File Upload Size
Already configured at 500MB in `web.config`

### Add Custom Domain (Optional)
```bash
az webapp config hostname add `
  --resource-group video-interpolation-rg `
  --webapp-name video-interpolation-app `
  --hostname yourdomain.com
```

---

## 💾 File Storage Options

### Current Setup (Development)
- Files stored locally in Azure App Service
- **Note**: Files deleted when app restarts (ephemeral storage)

### For Production: Use Azure Blob Storage

```bash
# Create storage account
az storage account create `
  --name videostg12345 `
  --resource-group video-interpolation-rg `
  --location eastus `
  --sku Standard_LRS

# Create blob container
az storage container create `
  --account-name videostg12345 `
  --name videos
```

Then update `server.js` to use Azure Blob Storage instead of local filesystem.

---

## 📈 Next Steps

1. ✅ **Deployed!** Your app is now live
2. Test it at: `https://video-interpolation-app.azurewebsites.net`
3. Set up **Application Insights** for monitoring
4. Configure **Auto-scaling** for production traffic
5. Add a **Custom Domain** with HTTPS
6. Set up **Backups** for reliability

---

## 🗑️ Cleanup (When Done)

To delete all Azure resources and stop incurring charges:

```bash
az group delete `
  --name video-interpolation-rg `
  --yes `
  --no-wait
```

---

## 📚 Additional Resources

- [Azure App Service Documentation](https://docs.microsoft.com/azure/app-service/)
- [Azure CLI Command Reference](https://learn.microsoft.com/cli/azure/)
- [Node.js on Azure](https://azure.microsoft.com/services/app-service/web/)
- [Troubleshooting Guide](https://learn.microsoft.com/azure/app-service/troubleshoot-common-app-service-errors)

---

## ❓ Need Help?

- **Azure Support**: [Create a support ticket](https://portal.azure.com/#blade/HubsExtension/ResourceMenuBlade/supportRequest)
- **GitHub Issues**: Create an issue in your repository
- **Stack Overflow**: Tag `azure` + `node.js`

---

**Happy Deploying! 🎉**

Your Video Frame Interpolation app is now accessible to the world!
