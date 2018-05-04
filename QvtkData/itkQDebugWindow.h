#ifndef __ITK_QDEBUQ_WINDOW_H__
#define __ITK_QDEBUQ_WINDOW_H__
#pragma once
// me 
#include "qvtkdata_export.h"

// itk
#include <itkOutputWindow.h>
#include <itkObjectFactory.h>

// qt
#include <QString>

namespace itk
{
// this class is used to send output to stdout and not the itk window
class QVTKDATA_EXPORT QDebugWindow:public OutputWindow
{
public:
  typedef QDebugWindow                 Self;
  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  /** Run-time type information (and related methods).   */
  itkTypeMacro(QDebugWindow, OutputWindow);

  /** New macro for creation of through a Smart Pointer   */
  itkNewMacro(QDebugWindow);

  virtual void DisplayText(const char *s) ITK_OVERRIDE;

protected:
  QDebugWindow();
  virtual ~QDebugWindow();

  QString String;

private:
  ITK_DISALLOW_COPY_AND_ASSIGN(QDebugWindow);
};
}

#endif // !__ITK_QDEBUQ_WINDOW_H__
