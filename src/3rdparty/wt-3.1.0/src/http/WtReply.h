// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2008 Emweb bvba, Kessel-Lo, Belgium.
 *
 * All rights reserved.
 */
//
#ifndef HTTP_WT_REPLY_HPP
#define HTTP_WT_REPLY_HPP

#include <string>
#include <vector>

#include "Reply.h"
#include "Connection.h"

namespace http {
namespace server {

class StockReply;
class HTTPRequest;
class WtReply;

typedef boost::shared_ptr<WtReply> WtReplyPtr;

/// A Wt application reply to be sent to a client.
class WtReply : public Reply
{
public:
  typedef void (*CallbackFunction)(void *cbData);

  WtReply(const Request& request, const Wt::EntryPoint& ep);
  ~WtReply();

  virtual void consumeRequestBody(Buffer::const_iterator begin,
				  Buffer::const_iterator end,
				  bool endOfRequest);

  void setContentType(const std::string& type);
  void setLocation(const std::string& location);
  void send(const std::string& text, CallbackFunction callBack, void *cbData);

  const std::string& cin() const { return cin_; }
  const Request& request() const { return request_; }

  virtual bool expectMoreData();

protected:
  const Wt::EntryPoint& entryPoint_;
  std::string       cin_;
  std::string       cout_, nextCout_;
  std::string       contentType_;
  std::string       location_;
  bool              responseSent_, sending_;
  volatile CallbackFunction  fetchMoreData_;
  void *            callBackData_;

  virtual status_type     responseStatus();
  virtual std::string     contentType();
  virtual std::string     location();
  virtual boost::intmax_t contentLength();

  virtual asio::const_buffer nextContentBuffer();  
};

} // namespace server
} // namespace http

#endif // WT_REPLY_HPP
