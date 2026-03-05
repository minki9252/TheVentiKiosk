#ifndef VENTICORE_GLOBAL_H
#define VENTICORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VENTICORE_LIBRARY)
#define VENTICORE_EXPORT Q_DECL_EXPORT
#else
#define VENTICORE_EXPORT Q_DECL_IMPORT
#endif

#endif // VENTICORE_GLOBAL_H
