/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  <copyright holder> <email>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef FM_FILEOPERATION_H
#define FM_FILEOPERATION_H

#include <QObject>
#include <libfm/fm.h>
#include "fileoperationdialog.h"

namespace Fm {

class FileOperationDialog;

class FileOperation : public QObject
{
Q_OBJECT
public:
  enum Type {
    Copy = FM_FILE_OP_COPY,
    Move = FM_FILE_OP_MOVE,
    Link = FM_FILE_OP_LINK,
    Delete = FM_FILE_OP_DELETE,
    Trash = FM_FILE_OP_TRASH,
    Untrash = FM_FILE_OP_UNTRASH,
    ChangeAttr = FM_FILE_OP_CHANGE_ATTR
  };

public:
  explicit FileOperation(Type type, FmPathList* srcFiles, QObject* parent = 0);
  virtual ~FileOperation();

  bool run();

  void cancel() {
    if(job)
      fm_job_cancel(FM_JOB(job));
  }

  bool isRunning() const {
    return job ? fm_job_is_running(FM_JOB(job)) : false;
  }

  bool isCancelled() const {
    return job ? fm_job_is_cancelled(FM_JOB(job)) : false;
  }

private:
  static gint onFileOpsJobAsk(FmFileOpsJob* job, const char* question, char* const* options, FileOperation* pThis);
  static gint onFileOpsJobAskRename(FmFileOpsJob* job, FmFileInfo* src, FmFileInfo* dest, char** new_name, FileOperation* pThis);
  static FmJobErrorAction onFileOpsJobError(FmFileOpsJob* job, GError* err, FmJobErrorSeverity severity, FileOperation* pThis);
  static void onFileOpsJobPrepared(FmFileOpsJob* job, FileOperation* pThis);
  static void onFileOpsJobCurFile(FmFileOpsJob* job, const char* cur_file, FileOperation* pThis);
  static void onFileOpsJobPercent(FmFileOpsJob* job, guint percent, FileOperation* pThis);
  static void onFileOpsJobFinished(FmFileOpsJob* job, FileOperation* pThis);
  static void onFileOpsJobCancelled(FmFileOpsJob* job, FileOperation* pThis);

  void disconnectJob();
  void showDialog();

private Q_SLOTS:
  void onShowDialogTimeout();
  
private:
  FmFileOpsJob* job;
  FileOperationDialog* dlg;
  QTimer* showUITimer;
};

}

#endif // FM_FILEOPERATION_H
