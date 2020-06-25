/*
This class should be inherited by other screens so that they follow the correct
blueprint Methods should be overriden if they will be used by that screen,
otherwise they will do nothing

Notes on C++ inheritance:
For a class to override a superclass's methods, those methods must be declared as virtual
This means that the methods are bound at runtime, so there is flexibility on what method is called (ie runtime polymorphism works)
If these methods below were declared as non-virtual, any call to the same method of a SUBCLASS would result in the method of the BASE CLASS being called, since the method binding is done at compile time
So by default all of these methods just do nothing, and it is up to subclasses to (optionally) implement alternate behaviour
This class is NOT an interface, since the methods are all implemented
An interface would have to have at least one "pure virtual" method, ie a method defined as `virtual void method() = 0`
This would declare the class as being abstract, so objects of that type could not be instantiated

TODO: convert to abstract class after more research
*/
class WatchScreenBase {
   public:
    virtual void screenSetup() {}
    virtual void screenDestroy() {}
    virtual void screenLoop() {}
    virtual void swipeLeft() {}
    virtual void swipeRight() {}
    virtual void swipeUp() {}
    virtual void swipeDown() {}
};