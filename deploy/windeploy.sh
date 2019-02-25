product_name="AshEngine"
display_name="Ash Engine"
release_dir=release
version_full=$1
version_main=`echo $version_full | cut -d'-' -f-1 | cut -d'v' -f2-`

find $release_dir -not -wholename "${release_dir}/${product_name}.exe" -delete

mkdir ${release_dir}/config
mkdir ${release_dir}/packages
mkdir ${release_dir}/packages/${product_name}
mkdir ${release_dir}/packages/${product_name}/meta
mkdir ${release_dir}/packages/${product_name}/data
mkdir ${release_dir}/packages/${product_name}/data/scripts

cp ${release_dir}/${product_name}.exe ${release_dir}/packages/${product_name}/data
cp lib/win/*.dll ${release_dir}/packages/AshEngine/data
windeployqt.exe ${release_dir}/packages/${product_name}/data/${product_name}.exe

cat > ${release_dir}/config/config.xml << EOF
<?xml version="1.0" encoding="UTF-8"?>
<Installer>
    <Name>${display_name}</Name>
    <Version>${version_full}</Version>
    <Title>${display_name} Installer</Title>
    <Publisher>Alfred Liu</Publisher>
    <StartMenuDir>${display_name}</StartMenuDir>
    <TargetDir>@ApplicationsDirX64@\\${display_name}</TargetDir>
</Installer>
EOF

cat > ${release_dir}/packages/${product_name}/data/scripts/auto_uninstall.qs << EOF
function Controller() {
    gui.clickButton(buttons.NextButton);
    gui.clickButton(buttons.NextButton);
    installer.uninstallationFinished.connect(this, this.uninstallationFinished);
}

Controller.prototype.uninstallationFinished = function() {
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.FinishedPageCallback = function() {
    gui.clickButton(buttons.FinishButton);
}
EOF

cat > ${release_dir}/packages/${product_name}/meta/installscript.qs << EOF
var targetDirectoryPage = null;

function Component()
{
    installer.gainAdminRights();
    component.loaded.connect(this, this.installerLoaded);
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    component.addOperation("CreateShortcut",
                           "@TargetDir@/${product_name}.exe",
                           "@DesktopDir@/${display_name}.lnk",
                           "workingDirectory=@TargetDir@");

    component.addOperation("CreateShortcut",
                           "@TargetDir@/${product_name}.exe",
                           "@StartMenuDir@/${display_name}.lnk",
                           "workingDirectory=@TargetDir@");
}

Component.prototype.installerLoaded = function()
{
    installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
    installer.addWizardPage(component, "TargetWidget", QInstaller.TargetDirectory);

    targetDirectoryPage = gui.pageWidgetByObjectName("DynamicTargetWidget");
    targetDirectoryPage.windowTitle = "Choose Installation Directory";
    targetDirectoryPage.description.setText("Please select where the ${display_name} will be installed:");
    targetDirectoryPage.targetDirectory.textChanged.connect(this, this.targetDirectoryChanged);
    targetDirectoryPage.targetDirectory.setText(installer.value("TargetDir"));
    targetDirectoryPage.targetChooser.released.connect(this, this.targetChooserClicked);

    gui.pageById(QInstaller.ComponentSelection).entered.connect(this, this.componentSelectionPageEntered);
}

Component.prototype.targetChooserClicked = function()
{
    var dir = QFileDialog.getExistingDirectory("", targetDirectoryPage.targetDirectory.text);
    targetDirectoryPage.targetDirectory.setText(dir);
}

Component.prototype.targetDirectoryChanged = function()
{
    var dir = targetDirectoryPage.targetDirectory.text;
    if (installer.fileExists(dir) && installer.fileExists(dir + "/maintenancetool.exe")) {
        targetDirectoryPage.warning.setText("<p style=\"color: red\">Existing installation detected and will be overwritten.</p>");
    } else if (installer.fileExists(dir)) {
        targetDirectoryPage.warning.setText("<p style=\"color: red\">Installing in existing directory. It will be wiped on uninstallation.</p>");
    } else {
        targetDirectoryPage.warning.setText("");
    }
    installer.setValue("TargetDir", dir);
}

Component.prototype.componentSelectionPageEntered = function()
{
    var dir = installer.value("TargetDir");
    if (installer.fileExists(dir) && installer.fileExists(dir + "/maintenancetool.exe")) {
        installer.execute(dir + "/maintenancetool.exe", "--script=" + dir + "/scripts/auto_uninstall.qs");
    }
}
EOF

cat > ${release_dir}/packages/${product_name}/meta/package.xml << EOF
<?xml version="1.0" encoding="UTF-8"?>
<Package>
    <DisplayName>${display_name}</DisplayName>
    <Description>A cross-platform 3D Engine for learning purpose.</Description>
    <Version>${version_main}</Version>
    <ReleaseDate></ReleaseDate>
    <Default>true</Default>
    <Script>installscript.qs</Script>
    <UserInterfaces>
        <UserInterface>targetwidget.ui</UserInterface>
    </UserInterfaces>
</Package>
EOF

cat > ${release_dir}/packages/${product_name}/meta/targetwidget.ui << EOF
<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>TargetWidget</class>
 <widget class="QWidget" name="TargetWidget">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>491</width>
    <height>190</height>
   </rect>
  </property>
  <property name="sizePolicy">
   <sizepolicy hsizetype="Preferred" vsizetype="Preferred">
    <horstretch>0</horstretch>
    <verstretch>0</verstretch>
   </sizepolicy>
  </property>
  <property name="minimumSize">
   <size>
    <width>491</width>
    <height>190</height>
   </size>
  </property>
  <property name="windowTitle">
   <string>Form</string>
  </property>
  <layout class="QVBoxLayout" name="verticalLayout">
   <item>
    <widget class="QLabel" name="description">
     <property name="text">
      <string/>
     </property>
    </widget>
   </item>
   <item>
    <layout class="QHBoxLayout" name="horizontalLayout">
     <item>
      <widget class="QLineEdit" name="targetDirectory">
       <property name="readOnly">
        <bool>true</bool>
       </property>
      </widget>
     </item>
     <item>
      <widget class="QToolButton" name="targetChooser">
       <property name="sizePolicy">
        <sizepolicy hsizetype="Fixed" vsizetype="Preferred">
         <horstretch>0</horstretch>
         <verstretch>0</verstretch>
        </sizepolicy>
       </property>
       <property name="minimumSize">
        <size>
         <width>0</width>
         <height>0</height>
        </size>
       </property>
       <property name="text">
        <string>...</string>
       </property>
      </widget>
     </item>
    </layout>
   </item>
   <item>
    <layout class="QHBoxLayout" name="horizontalLayout_2">
     <property name="topMargin">
      <number>0</number>
     </property>
     <item>
      <widget class="QLabel" name="warning">
       <property name="enabled">
        <bool>true</bool>
       </property>
       <property name="text">
        <string>TextLabel</string>
       </property>
      </widget>
     </item>
     <item>
      <spacer name="horizontalSpacer">
       <property name="orientation">
        <enum>Qt::Horizontal</enum>
       </property>
       <property name="sizeHint" stdset="0">
        <size>
         <width>40</width>
         <height>20</height>
        </size>
       </property>
      </spacer>
     </item>
    </layout>
   </item>
   <item>
    <spacer name="verticalSpacer">
     <property name="orientation">
      <enum>Qt::Vertical</enum>
     </property>
     <property name="sizeHint" stdset="0">
      <size>
       <width>20</width>
       <height>122</height>
      </size>
     </property>
    </spacer>
   </item>
  </layout>
 </widget>
 <resources/>
 <connections/>
</ui>
EOF

cd ${release_dir} && binarycreator.exe --offline-only -c config/config.xml -p packages installer.exe
