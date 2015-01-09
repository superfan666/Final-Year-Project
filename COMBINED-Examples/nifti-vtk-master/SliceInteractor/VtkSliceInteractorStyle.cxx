#include <VtkSliceInteractorStyle.h>
#include <StatusMessage.h>
#include <vtkRenderWindowInteractor.h>

void VtkSliceInteractorStyle::SetImageViewer(vtkImageViewer2* imageViewer) {
   _ImageViewer = imageViewer;
   _MinSlice = imageViewer->GetSliceMin();
   _MaxSlice = imageViewer->GetSliceMax();
   _Slice = _MinSlice;
   cout << "Slicer: Min = " << _MinSlice << ", Max = " << _MaxSlice << std::endl;
}

void VtkSliceInteractorStyle::SetStatusMapper(vtkTextMapper* statusMapper) {
   _StatusMapper = statusMapper;
}

void VtkSliceInteractorStyle::MoveSliceForward() {
   if(_Slice < _MaxSlice) {
      _Slice += 1;
      cout << "MoveSliceForward::Slice = " << _Slice << std::endl;
      _ImageViewer->SetSlice(_Slice);
      std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
      _StatusMapper->SetInput(msg.c_str());
      _ImageViewer->Render();
   }
}

void VtkSliceInteractorStyle::MoveSliceBackward() {
   if(_Slice > _MinSlice) {
      _Slice -= 1;
      cout << "MoveSliceBackward::Slice = " << _Slice << std::endl;
      _ImageViewer->SetSlice(_Slice);
      std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
      _StatusMapper->SetInput(msg.c_str());
      _ImageViewer->Render();
   }
}

void VtkSliceInteractorStyle::OnKeyDown() {
   std::string key = this->GetInteractor()->GetKeySym();
   if(key.compare("Up") == 0) {
      //cout << "Up arrow key was pressed." << endl;
      MoveSliceForward();
   }
   else if(key.compare("Down") == 0) {
      //cout << "Down arrow key was pressed." << endl;
      MoveSliceBackward();
   }
   // forward event
   vtkInteractorStyleImage::OnKeyDown();
}

void VtkSliceInteractorStyle::OnMouseWheelForward() {
   //std::cout << "Scrolled mouse wheel forward." << std::endl;
   MoveSliceForward();
   // don't forward events, otherwise the image will be zoomed 
   // in case another interactorstyle is used (e.g. trackballstyle, ...)
   // vtkInteractorStyleImage::OnMouseWheelForward();
}

void VtkSliceInteractorStyle::OnMouseWheelBackward() {
   //std::cout << "Scrolled mouse wheel backward." << std::endl;
   if(_Slice > _MinSlice) {
      MoveSliceBackward();
   }
   // don't forward events, otherwise the image will be zoomed 
   // in case another interactorstyle is used (e.g. trackballstyle, ...)
   // vtkInteractorStyleImage::OnMouseWheelBackward();
}

vtkStandardNewMacro(VtkSliceInteractorStyle);