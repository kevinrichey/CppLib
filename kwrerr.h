#ifndef KWR_INCLUDED_KWRERR_H
#define KWR_INCLUDED_KWRERR_H 

#include "kwrlib.h"

namespace kwr {

struct EventCategory {
    static const char Error[];
    static const char Defect[];
    static const char Fault[];
    static const char Failure[];
};

class Event {
  public:
    Event(SourcePoint source, CString message, CString cat) : 
      when(std::time(nullptr)), 
      where(source), 
      what(message),
      category(cat)
    {}

    std::time_t   when;
    SourcePoint   where;
    CString       what;
    CString       category;

    virtual void handle() =0;
    virtual void raise() const =0;
    virtual void print(kwr::OutStream& out) const;
};

class EventHandler {
  public:
    virtual void handle(Event& error) =0;
};

template <const char* cat, typename Base=Event>
class GenericEvent : public Base {
  public:
    GenericEvent(SourcePoint source, CString message, CString catname = cat) : 
      Base(source, message, catname)
    {}

    virtual void handle() { if (handler) handler->handle(*this); }
    virtual void raise() const { throw *this; }

    static EventHandler* handler;
};

template <const char* cat, typename Base>
EventHandler* GenericEvent<cat,Base>::handler = nullptr;


typedef GenericEvent<EventCategory::Error> Error;
typedef GenericEvent<EventCategory::Defect, Error> Defect;
typedef GenericEvent<EventCategory::Fault, Error> Fault;
typedef GenericEvent<EventCategory::Failure, Error> Failure;


class ThrowHandler : public EventHandler {
  public:
    virtual void handle(Event& error);
};

class PrintHandler : public EventHandler {
  public:
    virtual void handle(Event& error);
};

class DefectHandler : public EventHandler {
  public:
    virtual void handle(Event& error);
};

}; // kwr

#endif
