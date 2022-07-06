# Define the Rules & Notifications

Sometimes it may be useful to send the end-user a mobile app notification when their device reports a given state, such when batteries are low, a filter needs changing, or the wash is dry. Use this window to define the conditions (triggering events) and the resulting notification.

Defining rules and notifications is not mandatory, so you can skip this step if you wish.

## Create a Rule

A rule is defined by specifying at least one condition in which an attribute you’ve defined reports a certain value.

<ol class="af-ol">
	<li>To get started, click <span class="UIText">Rules &amp; Notifications</span>. The Rules &amp; Notification window opens:</p>
<img class="img-br" src="../img/Rules-Notifications.png" width="500" alt="Rules &amp; Notifications Window"></li>
	<li><p>Click <span class="UIText">+Rule</span>. The rule definition editor opens:</p>
	<img class="img-br" src="../img/NewRule.png" width="400" alt="Create New Rule"></li>
	<li>Give your new rule a <span class="UIText">Rule Name</span>.</li>
	<li>If you already have a notification defined that you can use for this rule, select it from the <span class="UIText">Notification</span> drop-down menu. If not, create a new notification:</li>
	<ol class="af-ol-lower-alpha">
		<li><p>In the right-hand <span class="UIText">Notifications</span> pane, select <span class="UIText">+Notification</span>. The New Notification window opens:</p>
		<img class="img-br" src="../img/NewNotification.png" width="400" alt="Create New Notification"></li>
		<li>Describe your notification briefly in the <span class="UIText">Description</span> field. This description is what will appear in the drop-down menu in the rule definition editor and is only used internally (not exposed to end-users).</li>
		<li>In the <span class="UIText">Message</span> field, type the message you want the end-user to receive on their smartphone. Messages are limited to 255 characters.</li>
		<li>Click <span class="UIText">Save</span> when you’re done with the message. You can always edit or disable the message after you’ve saved it.</li>
		<li>If you have a notification you’ve already defined, you can use it as a template for a new one by selecting the duplicate icon:<img class="img-br" src="../img/DupNotification.png" width="200"></li>
		</ol>
	<li><p>Define the <span class="UIText">Condition(s)</span>, or triggering events, by selecting an <span class="UIText">Attribute</span> and the corresponding <span class="UIText">Value</span> it must report to trigger the notification.</p>
		<div class="af-callout">
			<div class="callout-text">
			<p><img src="../img/Note.svg" width="17" style="vertical-align:bottom;padding:0">&nbsp;&nbsp;<strong>NOTE:</strong>  If you list more than one condition, <strong>all</strong> conditions must be met before a notification is triggered.</p>
			</div>
		</div>
	</li>
	<li>To create another rule, simply click <span class="UIText">+Rule</span>.</li>
	<li>When you’re finished defining all your rules, click <span class="UIText">Save</span> in the upper-right corner of the window.</li>			
</ol>
			
## Manage Rules & Notifications

You can always modify your rules & notifications definition:
<ul class="af-ul">
<li>
<strong>Edit a Rule</strong> - If your rule is collapsed, click the Rule Name to open its definition editor. Make your edits, then click the Rule Name to collapse it again.
</li><li>
<strong>Delete a Rule</strong> - Locate the rule you want to delete, then click the trash icon:<img src="../img/TrashIconWhite.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none">. You’ll be asked to confirm.
</li><li>
<strong>Edit a Notification</strong> - Locate the notification you want to edit, then click the pencil icon <img src="../img/PencilIconWhite.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none">. Make your edits, then click <span class="UIText">SAVE</span>.
</li><li>
<strong>Duplicate a Notification</strong> - Locate the notification you want to duplicate and click the duplicate icon<img src="../img/DupIcon.png" width="25" style="vertical-align:middle;margin:0px 0px;border:none">, located on the right. A new notification window opens using the Description and Message from the original notification. Make your edits, then click <span class="UIText">SAVE</span>.
</li></ul>

