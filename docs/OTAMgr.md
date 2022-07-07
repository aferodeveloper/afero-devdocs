# OTA MANAGER USER GUIDE

Afero offers developers the ability to send over-the-air (OTA) firmware image updates to the MCUs in their developer devices. The type of firmware image you want to update is up to you, the developer.

At a high level, this is how it works:

<ol class="af-ol">
	<li>Developer uses the Afero Profile Editor to publish a Profile to devices of a given Device Type.</li>
	<li>Developer uses the OTA Manager to create the firmware “Image Types” they will be using and associates each Image Type with a Device Type.</li>
	<li>Developer uses the Profile Editor to enable MCU Firmware OTA Updates for selected firmware Image Types.</li>
	<li>Developer uses the OTA Manager to upload the firmware image (associating it with one of their Image Types), where it is hosted on the Afero OTA server.</li>
	<li>Developer uses the OTA Manager to “deploy” the firmware image to eligible devices.</li>
	<li>Afero uses its OTA service to send the OTA update to the ASR in the device.</li>
	<li>The ASR passes the firmware update to the MCU using the ASR &#8646; MCU interface.</li>
	<li>Update success/failure is reported back to Afero.</li>
</ol>

<div class="af-callout">
	<div class="callout-text">
	<p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>NOTE:</strong> In addition, you'll have to prepare your application code to accept an MCU OTA update. Instructions are on this page: <a href="../MCU-OTA">Handling MCU OTA Updates</a>.
	</div>
</div>

## The OTA Manager and the Profile Editor Work Together

You can see that the Afero OTA Manager and the Afero Profile Editor work together to make MCU firmware OTA updates happen:

<ul class="af-ul">
	<li>The Profile Editor needs to know about the firmware “Image Type” (defined in the OTA Manager) <em>before</em> the developer can select it (in the Attributes Definition window) to receive MCU OTA firmware updates. The Image Type is used to define an attribute that allows the update to pass through ASR to the MCU on the device.</li>
	<li>The OTA Manager must know the device Profile’s “Device Type” (defined in the Profile Editor) <em>before</em> the firmware image can be uploaded to the OTA service. Device Type is associated with a firmware image to determine the devices eligible for the image update.</li>
</ul>

So, before you upload a firmware image, you must have used the Profile Editor to create and publish at least one device Profile that has at least one Image Type selected on the Attributes Definition window in the MCU Configuration pane, Firmware OTA Updates section. However, before a Profile can be published, the developer must have defined at least one Image Type.

Also note that:

<ul class="af-ul">
	<li>Device Profiles can be associated with one or more Image Types.</li>
	<li>Firmware images can be associated with one or more Device Types.</li>
</ul> 

## Sign In Window

The Afero OTA Manager is a web application, available from [https://otamanager.afero.io](https://otamanager.afero.io/). Sign in using your Afero developer account credentials, the same email and password you use to sign in to the Afero Profile Editor.

<img src="../img/OTA-login.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

## Create Firmware Image Types

Before you can upload a firmware image, you must define at least one **Image Type**. An Image Type is a category of firmware image; you can name them however you wish but make sure you are creating a “category” or type of image, rather than identifying a specific image destined for a specific device.

Image types are shown in the OTA Manager as folders; again, you should name them based on the “types” of firmware images you plan to use for your MCU updates. Types could include bootloader, application, or any specific identifier you choose to categorize your firmware.

<div class="af-callout">
	<div class="callout-text">
	<p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>NOTE:</strong> You can create up to <strong>100 Image Types</strong>.
	</div>
</div>

### First-Time Flow

The first time you sign in to the Afero OTA Manager, you will be taken through a wizard that helps you create at least one firmware Image Type. If you know all the types you’ll be using, you are encouraged to create them all during this initial setup. You can always add more, or edit the name/description of Image Types later.

### Add Firmware Image Type

After the first sign-in wizard flow, you will be able to create Image Types from the main window of the OTA Manager:

<ol class="af-ol">
	<li>From the upper-right of the OTA Manager main window, select the <span class="UIText">Add</span> button, just to the right of the heading <span class="UIText">Image Types</span>. The Add Firmware Image Type window appears:</p>
	<p><img class="img-br" src="../img/NewImageType.png" width="300" alt="Add Firmware Image Type"></p></li>
	<li>Give your Image Type a name and description, then click <span class="UIText">Add</span>.</li>
</ol>


### Edit Existing Image Type

If you want to change the name or description of an existing Image Type, you can do this from the main window. Note that once you’ve created an Image Type, that type is assigned an ID that does not change even when you edit the name or description. So, if what you want is the underlying category of the image to be different, create a new Image Type rather than changing the name on an existing type.

<ol class="af-ol">
	<li>From the OTA Manager main window on the left, hover your cursor over the pencil icon <img class="img-inline" src="../img/EditIcon.png" style="vertical-align:middle" width="20"> to the left of an existing Image Type. The option to <span class="UIText">Edit Image Type</span> will appear. Select the icon.</li>
	<li>The Edit Firmware Image Type window will appear. Make your edits and <span class="UIText">Save</span>.</li>
</ol>

## Upload a Firmware Image

When you are ready to upload a firmware image, start from the OTA Manager main window.

<div class="af-callout">
	<div class="callout-text">
	<p><img src="../img/Note.svg" width="15" style="vertical-align:bottom;padding:0"> <strong>NOTE:</strong> Remember that before uploading an image, you must have already used the Profile Editor to create and publish a device Profile that has a firmware Image Type selected. You do this on the Attributes Definition window > MCU Configuration pane > Firmware OTA Updates section. Read <a href="../AttrDef#define-the-mcu-attributes">Define the MCU Attributes</a> for more details.
	</div>
</div>

<ol class="af-ol">
	<li>From the upper-right of the window, select <span class="UIText">+Upload New Image</span>. The three-step upload wizard appears.</li>
	<li>You can upload your image in either of two ways:
		<ul class="af-ul">
			<li>Drag & drop the image into the target Image Type folder; or</li>
			<li>Select the target Image Type folder then navigate to your image file and select it.</li>
		</ul>
		<p>When finished, click <span class="UIText">Next: Add info for your firmware</span>.</p>
	</li>
	<li>
		<p>On the next window you’ll add some metadata for your image: image name, description, version string, and tag(s). You will be able to search on this information later if you want to find a specific image quickly, or use one of the fields to filter your images. Press Enter after typing a tag to add more tags.</p>
		<p>When finished, click <span class="UIText">Next: Associate Device Types</span>.</p>
	</li>
	<li>All your eligible Device Types appear on the next window. Select those you want to receive the firmware image. Select <span class="UIText">DONE</span> when you’re finished.</li>
</ol>

## Deploy a Firmware Image

Once you’re ready to deploy your uploaded firmware image, you do so from the main window.

### Select Image

The main window of the OTA Manager displays all your images. If you have a large number of images, you may want to limit the images shown:

<ul class="af-ul">
	<li>Use the left-hand Navigation pane to filter images based on Image Type or Device Type.</li>
	<li>Use the <span class="UIText">Search My Tags</span> box to search on one or more image tags you’ve assigned.</li>
</ul>

### Push Image

Once you have found the firmware image you want to deploy, you’re ready to push the image to your device(s):

<ol class="af-ol">
	<li>With the firmware image you want to deploy selected, click the <span class="UIText">Deploy</span> button to the far-right of the firmware image entry.</li>
	<li>Below the image name, the target device ID(s) are shown. Select the targets.</li>
	<li>Click the <span class="UIText">Push Now</span> button on the far-right.</li>
	<li><p>You can see deployment status in a message (in orange text) below your selected device(s):</p>
	<img class="img-br" src="../img/OTA-DeployStatus.png" width="350" alt="Deployment Status"></li>
</ol>

## See Also

- Read [Handling MCU OTA Updates](../MCU-OTA) for details on what you’ll need to include in your application code to accept an MCU OTA update.
- Read [Define the MCU Attributes](../AttrDef#define-the-mcu-attributes) for details on using the Profile Editor to prepare your MCU for accepting specific Image Types.
