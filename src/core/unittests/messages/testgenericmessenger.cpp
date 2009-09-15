// What to test :
// - can add listener
// - can add same listener with two different msgtype
// - cannot add same listener with same msgtype
// - can add different listeners with same msgtype
//
// - can delete added listener for message type
// - cannot delete added listener for wrong message type
// - cannot delete non added listener
//
// - cannot trigger an unknown type (no listener added)
// - cannot trigger an unknown type (listener added)
// - with two listeners, trigger calls listener is message type ok
// - with two listeners, trigger does not call listener is message type not ok
//
// - cannot queue an unknown type (no listener)
// - cannot queue an unknown type (listener added)
// - with two listeners, queue doesn't not call any listener
//
// - queue message A, process. Listener for A is called
// - queue messages A B, process, Listener for A B is called
// - queue messages A B, process, Listener for A and Listener for B is called
//
// - queue three messages of type A B A (needs two listeners)
// -- aborts first A, process, both listeners called once
// -- aborts all A, process, B listener called once
// -- aborts first B, process, A listener called twice
// -- aborts all B, process, A listener called twice
//
// Bonus :
// - Register a listen to anything, listen to message A. Will throw
//
//
// Needs Listener Mock
// Needs Message Mock

