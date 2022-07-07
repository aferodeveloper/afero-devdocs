# CONSOLE USER GUIDE

Afero Console is designed for partners so they can monitor and visualize their device deployments and usage. It offers aggregated, real-time location views of partner devices as well as individual device use history.

The Console is easy to use and self-documenting. A description of the user interface is presented on this page. Be sure to refer to the [Glossary](../Glossary) for term definitions when using the Console.

## Sign In and Out of Afero Console

Go to <a href="https://console.afero.io" target="_blank">https://console.afero.io</a> then sign in with your Afero partner username (email) and password.

To sign out, click the arrow <img src="../img/Console-SignOut.png" width="23" style="vertical-align:middle;margin:0px 0px;border:none"> in the upper-right of the main Console window.

## Switch Partners

If you belong to more than one partner, you can switch to view the Console of a different partner. To do so, click your username in the upper-left of any Console window. Your partner names will appear; select the partner whose Console you want to view. You can also edit your partner name by clicking <span class="UIText">UPDATE PARTNER NAME</span>.

## The Console Windows

To view any of the windows in Afero Console, use the left-hand Navigation pane. In addition to the Real-Time Activity window, which opens by default, you’ll find links to Device Listing, Device Type Reports (click to see all your Device Types), Platform Usage, Platform Status, the Developer Portal, and the Dev Portal page with links for purchasing Afero hardware. The Console windows are described and illustrated below:

- [Real-Time Activity](../Console#real-time-activity-window)
- [Device Listing](../Console#device-listing) and [Individual Device Data Viewer](../Console#individual-device-data-viewer)
- [Device Type Reports](../Console#device-type-reports-window)
- [Data Explorer](../Console#data-explorer)
- [Platform Usage](../Console#platform-usage-window)
- [Platform Status](../Console#platform-status-window)
- [User Manager](../Console#user-manager-window)

### Real-Time Activity Window

Click <span class="UIText">REAL-TIME ACTIVITY</span> in the left-hand Navigation pane to open a window displaying by location the activity, or lack of activity, taking place on your devices in the field (your fleet) in real-time. Only devices experiencing an event will display. Device indicators are color-coded by event class, or type:

- Device attribute state has just changed.
- Device is on, connected to the Afero Platform, and available.
- Device has disconnected from the Afero Platform, perhaps off, definitely not connected to the network, or otherwise unavailable.

The number of events occurring, by activity type, are displayed in the key below the map.

You can switch between a map of the world (Global Map) or United States (USA Map). Zoom in/out, go full screen, switch among map, satellite, terrain, and street views, or jump to a map location, by using the usual Google Map controls.

To clear the map of all device activity, click the <span class="UIText">CLEAR MARKERS</span> button in the upper-right corner.

<img src="../img/Console-RealTime.png" width="700" style="vertical-align:middle;margin:0px 0px;border:none">

### Device Listing

Click <span class="UIText">DEVICE LISTING</span> in the left-hand Navigation pane to open a window displaying all your devices, including methods for filtering and finding a specific device or set of devices.

In the table titled <span class="UIText">YOUR AFERO POWERED DEVICE LISTING</span>, click a Device ID to open the [Individual Device Data Viewer](../Console#individual-device-data-viewer), or click a <span class="UIText">DEVICE TYPE NAME</span> to go to the relevant [Device Type Reports](../Console#device-type-reports-window) window.

You can package and download the contents of this device listings table (with additional Creation Date, Profile ID, and Location information) into a zipped .csv file. Click the download icon on the far right <img src="../img/Console-DownloadIcon.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none"> then click the download link that appears. When unzipping the file, you’ll be prompted to enter the password provided under the download link (use copy & paste).

<img src="../img/Console-DeviceListing.png" width="700" style="vertical-align:middle;margin:0px 0px;border:none">

#### Individual Device Data Viewer

To view usage data on an **individual device**, do any of the following:

- From the Device Listing window, click a Device ID, *or*
- Type the Device ID in the Search box at the top of the main window, *or*
- From the Real-Time Activity window, click an event as it’s blinking. A summary of the event will appear, with a link to Device Details.

On successful search or navigation, the <span class="UIText">DEVICE DETAILS</span> tab opens. The <span class="UIText">ATTRIBUTE BROWSER</span> tab displays each attribute definition, how this attribute is presented on the mobile UI, and attribute update history (updates that occurred in the last 31 days, up to 200 entries).

<img src="../img/Console-DeviceDetails.png" width="700" style="vertical-align:middle;margin:0px 0px;border:none">

<img src="../img/Console-AttributeBrowser.png" width="700" style="vertical-align:middle;margin:0px 0px;border:none">

### Device Type Reports Window

Click <span class="UIText">DEVICE TYPE REPORTS</span> in the left-hand Navigation pane to show a list of all your Device Types, then select a specific Device Type to view its usage metrics.

Note that metrics are shown in date ranges; click (in the upper-right) <span class="UIText">FROM THIS WEEK</span>, <span class="UIText">FROM THIS MONTH</span>, or <span class="UIText">SELECT DATE RANGE</span> (to customize the range):

<img src="../img/Console-DateScope.png" width="700" style="vertical-align:middle;margin:0px 0px;border:none">

You’ll be able to view your Device Type usage in the following ways:

- [Device Type Status](../Console#device-type-status)
- [Attribute Usage Breakdown](../Console#attribute-usage-breakdown)
- [Attribute Value Breakdown](../Console#attribute-name-value-breakdown)
- [Relative Associated Device Distribution](../Console#relative-associated-device-distribution-map)
- [Cumulative Device Association Trend](../Console#cumulative-device-association-trend)
- [Application Version Breakdown](../Console#application-version-breakdown)
- [Connection Type Breakdown](../Console#connection-type-breakdown)

#### Device Type Status

At the top are four metrics: number of devices currently associated (attached to an end-user account) and pending association (produced but not yet associated), and total number of devices currently online (available) and offline (unavailable).

<img src="../img/Console-Online.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

#### Attribute Usage Breakdown

This table displays, for a selected Device Type, each partner-defined attribute’s usage, important information for determining how customers are using a specific type of device. In the example, the attributes compared are EcoMode, Heating Level, and Fan Speed. You can see that Fan Speed has changed more than the others.

<img src="../img/Console-DeviceType1.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

#### <*Attribute Name*> Value Breakdown

To the right of the Attribute Usage table is a donut graph or bar chart showing how end-users have used each of the attributes’ values.

To start, click an attribute in the Attribute Usage table, then look right to the graph or chart to see how popular that attribute’s values have been, based on how many times the customers have changed the value in the given timeframe.

- To view the popularity of a subset of values, select only the desired values using the <span class="UIText">VIEW IN CHART</span> checkboxes in the table below the graph.
- To see the total number of times that an attribute value was selected in the given timeframe, hover over an attribute value in the graph or look in the <span class="UIText">TOTAL RECORDS</span> column in the table.
- Attribute values are identified in this table using their labels, or <span class="UIText">VALUE OPTIONS</span>. These labels are displayed to the end-user in the mobile app UI, as control options.

    <img src="../img/Console-DeviceType2.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

#### Relative Associated Device Distribution Map

Scroll down the page to see the Relative Associated Device Distribution map. Use this map to see at a glance where all your associated devices are located, at this point in time.

You can switch views between United States only or Global by toggling the icon<img src="../img/Console-MapViewToggle.png" width="50" style="vertical-align:middle;margin:0px 0px;border:none">in the upper-right of the window.

<img src="../img/Console-RelAssocDeviceDistr.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

#### Cumulative Device Association Trend

Scroll further down the window to see the cumulative rate of device association trend over time.

<img src="../img/Console-CumAssocDeviceDistr.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

#### Application Version Breakdown

Scroll down to see a graph displaying the versions running on the associated devices. To report a version number, a device must have been associated *and* connected to the Afero Cloud. Those reporting an unknown version have not yet connected to the Cloud.

<img src="../img/Console-AppVersBkdwn.png" width="700" style="vertical-align:middle;margin:0px 0px;border:none">

#### Connection Type Breakdown

To find out how associated devices have connected to the Afero Cloud, Bluetooth, or Wi-Fi, scroll down to the Connection Type Breakdown graph.

<img src="../img/Console-ConnTypeBkdwn.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

### Data Explorer

Click <span class="UIText">DATA EXPLORER</span> on the left-hand Navigation pane to run your own SQL-92 compliant queries across all your devices’ historical data. Each query is run across multiple servers, returning results quickly and efficiently.

#### SQL Query Credits

Afero will give you a monthly allocation of Query Credits. Each query you run is charged on a per byte basis, where every byte of data that is evaluated in the query is measured and charged. Note that before your queries are run, they are evaluated; query evaluation is not charged against your credits. Each query evaluation will also result in a projected cost of Query Credits.

Your remaining monthly credits will always appear at the top of the Data Explorer Home window, along with other useful information:

<img src="../img/Console-QueryCredits.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

#### Enter a Query

There are a few ways you can query your data:

- Type your SQL query in the SQL Query field at the top of the Data Explorer Home window (shown above where you see “Type your SQL query here …”). Use the <span class="UIText">TABLE SCHEMA REFERENCE</span> tab for field names, and you can find a good SQL reference at this site: <a href="https://cloud.google.com/bigquery/docs/reference/standard-sql/" target="_blank">Google Cloud Big Query Standard SQL Reference</a>.

- Construct your SQL query using one of the <span class="UIText">SQL BUILDER TOOLS</span> in the upper-right corner of the Data Explorer Home window:

    <img src="../img/Console-QueryBuilder.png" width="300" style="vertical-align:middle;margin:0px 0px;border:none">

    Select one of the tools to begin:

    - **GEOLOCATION TOOL** - Click <span class="UIText">START DRAWING</span>, then click a location on the map to drop your first pin, clicking and dropping additional pins to define your geolocation boundary. Close the boundary by clicking the first (blue) pin you dropped. Once defined, you can fine-tune your geolocation by dragging the pins. You can always clear the map and start over by clicking the trash icon. When finished, click <span class="UIText">SAVE</span>, and once you’ve saved your query, it will appear in the SQL Query field.
    - **TIME SELECTOR** - Select a time and date range for your query, then click <span class="UIText">SAVE</span>. Your query is ready to run. Note that range defaults to the last 24 hours, and all times are shown in UTC (Coordinated Universal Time).
    - **QUERY BUILDER** - Build your query by first selecting the fields that you’re interested in. Then define your conditions using the selectors. Once you click <span class="UIText">SAVE</span>, your query will appear in the SQL Query field.<br><br>

- **Load a SQL query template provided in the QUERY TEMPLATES tab:**

    <img src="../img/Console-QueryTemplates.png" width="800" style="vertical-align:middle;margin:0px 0px;border:none">

    To select a query and have it appear in the SQL Query field, click the <span class="UIText">LOAD QUERY</span> button. In the examples below, we will use the query in the template, “Find the Top 10 Most Active Devices”.

#### Run a Query

Once you have a query in the SQL Query field, simply select the <span class="UIText">VALIDATE & PRICE</span> button. The results will appear in the upper far-right pane:

<img src="../img/Console-QueryValid.png" width="800" style="vertical-align:middle;margin:0px 0px;border:none">

If your query is valid and you have enough Query Credits, you can go ahead and select the <span class="UIText">EXECUTE QUERY JOB</span> button to proceed. If your query is not valid or you do not have sufficient Query Credits, you will see a notification describing the error or problem.

#### View Query Results

Results are displayed in the <span class="UIText">RESULTS</span> tab, on the far right of the tab bar. Each query you run is displayed in a numbered <span class="UIText">RESULTS</span> tab. You can clear any of the results by clicking <img src="../img/Console-ClearResults.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none"> in the tab.

By default, results are displayed in tabular format:

<img src="../img/Console-Results-Tabular.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

You can change the display by clicking the icons on the right:

| TABULAR<br><img src="../img/Console-TabularDisplay.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none"> | <img src="../img/Console-Tabular.png" width="175" style="vertical-align:middle;margin:0px 0px;border:none">| LINE<br><img src="../img/Console-LineDisplay.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none"> | <img src="../img/Console-Line.png" width="175" style="vertical-align:middle;margin:0px 0px;border:none"> |
  | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
  | DONUT<br><img src="../img/Console-DonutDisplay.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none"> | <img src="../img/Console-Donut.png" width="175" style="vertical-align:middle;margin:0px 0px;border:none"> | AREA<br><img src="../img/Console-AreaDisplay.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none"> | <img src="../img/Console-Area.png" width="175" style="vertical-align:middle;margin:0px 0px;border:none"> |
  | BAR<br><img src="../img/Console-BarDisplay.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none"> | <img src="../img/Console-Bar.png" width="175" style="vertical-align:middle;margin:0px 0px;border:none"> |                                                              |                                                              |

For all but the tabular format, you can select which columns are used in the display. Click the <span class="UIText">SAVE</span> button to save the query to use another time. Your saved queries will be listed in the Query Templates tab > Your Custom Query Templates.

#### Your Query History

Your past queries (up to 200) are saved in the <span class="UIText">YOUR QUERY HISTORY</span> tab, including when the query was run, the query text, time to execute, and cost. You can also conveniently run it again from this location.

#### Table Schema Reference

To view the details on what data is stored for your devices and how to reference the data in your queries, click the <span class="UIText">TABLE SCHEMA REFERENCE</span> tab.

### Platform Usage Window

Click <span class="UIText">PLATFORM USAGE</span> on the left-hand Navigation pane to display month-to-date details for each Device Type:

- Total monthly message count
- Total unique active devices
- Monthly per device (message) average

Totals across Device Types are also provided.

### Platform Status Window

You can view the current status of the Platform by clicking <span class="UIText">PLATFORM STATUS</span> in the left-hand Navigation pane or the checkmark icon<img src="../img/Console-checkmark.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none">.

Status is checked automatically every minute and reported as healthy if all subsystems are operating as expected. A degraded status means some part of the Platform has reported a less than optimal status, and also means the Operations Team has been notified.

### User Manager Window

This feature allows partners to invite users to view and use their Console. Users are invited with certain privileges, which partners can always edit or revoke.

#### Invite Users

To invite a user, select <span class="UIText">USER MANAGER</span> &gt; <span class="UIText">INVITE USER</span> from the left-hand Navigation bar. On the window that opens, type the email address in the @ field, then click <span class="UIText">NEXT</span>. The <span class="UIText">SET USER PRIVILEGES</span> pane appears:

<img src="../img/Console-InviteUser.png" width="500" style="vertical-align:middle;margin:0px 0px;border:none">

You can grant the following partner privileges; note that all privileges are ON by default:

- **INVITE OTHER USERS** - This user will be able to extend invitations to others to access your Console.

- **MANAGE DEVICE PROFILES** - This user will be able to add, update, and remove device Profiles using the Afero Profile Editor.

- **VIEW REAL-TIME DEVICE DETAILS** - This user will be able to view real-time, device-specific details.

- **VIEW REPORTS & RUN QUERIES** - This user will be able to use all the Console reporting tools.

Turn ON or OFF the privileges you want to grant this user, then select <span class="UIText">SEND USER INVITATION</span>.

The user will receive the invitation with either a link to sign in to Afero Console, or if they don’t already have an Afero account, a link to register. Once registered, they will automatically be signed in to Console.

#### Edit Privileges

Once you have invited at least one user who has accepted your invitation, you can change the privileges you have given them at any time. Select <span class="UIText">USER MANAGER</span> &gt; <span class="UIText">EDIT PRIVILEGES</span> from the left-hand Navigation bar. After selecting the user you want to modify, simply turn On or Off the privilege(s) you want to change; or, you can <span class="UIText">REMOVE USER</span>. Once you are finished editing, select the <span class="UIText">SAVE PRIVILEGES</span> button.

<img src="../img/Console-EditPriv.png" width="700" style="vertical-align:middle;margin:0px 0px;border:none">