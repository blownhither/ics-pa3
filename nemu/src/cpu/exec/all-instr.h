#include "prefix/prefix.h"

#include "data-mov/mov.h"
#include "data-mov/xchg.h"
#include "data-mov/cdq.h"
#include "data-mov/movsx.h"
#include "data-mov/movzx.h"
#include "data-mov/cmovcc.h"
#include "data-mov/cbw.h"

#include "arith/dec.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"
#include "arith/sub.h"
#include "arith/add.h"
#include "arith/adc.h"
#include "arith/sbb.h"

#include "binary/cmp.h"
#include "binary/and.h"

//#include "logic/and.h"
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"
#include "logic/shld.h"
#include "logic/test.h"
#include "logic/setcc.h"

#include "control/call.h"
//#include "control/callr.h"
#include "control/push.h"
#include "control/pop.h"
#include "control/jmp.h"
#include "control/leave.h"
#include "control/ret.h"
#include "control/jcc.h"
#include "control/cld.h"

#include "io/in.h"
//#include "io/out.h"

#include "string/rep.h"
#include "string/stos.h"
#include "string/cmps.h"
#include "string/movs.h"

#include "system/ldt.h"
#include "system/mov_special.h"
#include "system/ljmp.h"
#include "system/int.h"
#include "system/pusha-popa.h"
#include "system/iret.h"
#include "system/cli.h"

#include "misc/misc.h"

#include "special/special.h"
