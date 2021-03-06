set(SRC_CPP_FILES
	Util.cpp
  UncertaintyPreprocessor.cpp
  UncertaintyThresholder.cpp
  UncertaintySampler.cpp
  UncertaintyTextureGenerator.cpp
  SurfaceGenerator.cpp
  UncertaintySurfaceMapper.cpp
  UncertaintyGenerator.cpp
  RANSACScanPlaneGenerator.cpp
  SVDScanPlaneGenerator.cpp
  ColourLegendOverlay.cpp
  ScanSimulator.cpp
  MitkLoadingBarCommand.cpp
  QmitkCmdLineModuleFactoryGui.cpp
  QmitkCmdLineModuleGui.cpp
  QmitkDataStorageComboBoxWithSelectNone.cpp
  QmitkUiLoader.cpp
  SamsPointSet.cpp
)

set(INTERNAL_CPP_FILES
  com_sam_finalyearproject_Activator.cpp
  Sams_View.cpp
)

set(UI_FILES
  src/internal/Sams_ViewControls.ui
)

set(MOC_H_FILES  
  src/QmitkCmdLineModuleFactoryGui.h
  src/QmitkCmdLineModuleGui.h
  src/QmitkDataStorageComboBoxWithSelectNone.h
  src/QmitkUiLoader.h
  src/internal/com_sam_finalyearproject_Activator.h
  src/internal/Sams_View.h
)

# list of resource files which can be used by the plug-in
# system without loading the plug-ins shared library,
# for example the icon used in the menu and tabs for the
# plug-in views in the workbench
set(CACHED_RESOURCE_FILES
  resources/icon.xpm
  plugin.xml
)

# list of Qt .qrc files which contain additional resources
# specific to this plugin
set(QRC_FILES
  resources/Sams_View.qrc
)

set(CPP_FILES )

foreach(file ${SRC_CPP_FILES})
  set(CPP_FILES ${CPP_FILES} src/${file})
endforeach(file ${SRC_CPP_FILES})

foreach(file ${INTERNAL_CPP_FILES})
  set(CPP_FILES ${CPP_FILES} src/internal/${file})
endforeach(file ${INTERNAL_CPP_FILES})

