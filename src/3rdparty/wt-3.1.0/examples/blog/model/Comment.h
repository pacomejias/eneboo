// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2009 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef COMMENT_H_
#define COMMENT_H_

#include <Wt/WDateTime>

#include <Wt/Dbo/Types>
#include <Wt/Dbo/WtSqlTraits>

class Comment;
class Post;
class User;

namespace dbo = Wt::Dbo;

typedef dbo::collection<dbo::ptr<Comment> > Comments;

class Comment {
public:
  dbo::ptr<User>    author;
  dbo::ptr<Post>    post;
  dbo::ptr<Comment> parent;

  Wt::WDateTime     date;
  Wt::WString       textSrc;
  Wt::WString       textHtml;

  Comments          children;

  template<class Action>
  void persist(Action& a)
  {
    dbo::field(a, date, "date");
    dbo::field(a, textSrc, "text_source");
    dbo::field(a, textHtml, "text_html");

    dbo::belongsTo(a, post, "post");
    dbo::belongsTo(a, author, "author");
    dbo::belongsTo(a, parent, "parent");

    dbo::hasMany(a, children, dbo::ManyToOne, "parent");
  }
};

DBO_EXTERN_TEMPLATES(Comment);

#endif // COMMENT_H_
