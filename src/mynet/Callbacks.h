/*
 * Callback.h
 *
 *  Created on: 2013-8-13
 *      Author: fanbin
 */

#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include "../base/Buffer.h"
#include "../base/Timestamp.h"
#include "TcpConnection.h"
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

class TcpConnection;

namespace Callbacks
{
typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;

typedef boost::function<void(const TcpConnectionPtr&)> ConnectionCallback;
// the data has been read to (buf, len)
typedef boost::function<void(const TcpConnectionPtr&, Buffer*, Timestamp)> MessageCallback;
}

#endif /* CALLBACKS_H_ */
