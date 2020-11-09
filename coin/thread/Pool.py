#
# Pool.py
# (c) 2020 - 2021 willmbaker. All rights reserved.
#

import sys
import threading

import plumbum #@!- This dependency is unfortunate, and could be replaced if you could define which exceptions to catch on the consumer threads.


class Pool:
    class Work(object):
        def __init__( self, callback ):
            self.callback_ = callback

        @property
        def callback( self ):
            return self.callback_

    def __init__( self ):
        self.work_available_ = threading.Condition()
        self.work_complete_  = threading.Condition()
        self.should_exit_    = False
        self.work_           = []
        self.threads_        = []

    def start( self, number_of_threads ):
        self.threads_ = []

        for i in range(0,number_of_threads):
            self.threads_.append( threading.Thread(group=None, target=self.consumer_thread) )
            self.threads_[-1].start();

    def stop( self ):
        with self.work_available_:
            self.should_exit_ = True
            self.work_available_.notify_all()

        for thread in self.threads_:
            thread.join( timeout=10 )
            assert not thread.is_alive(), "The thread timed out waiting to join"

    def wait_for_all( self ):
        while len(self.work_):
            with self.work_complete_:
                self.work_complete_.wait()

    def add( self, callback ):
        with self.work_available_:
            self.work_.append( Pool.Work(callback) )
            self.work_available_.notify()

    def consumer_thread( self ):
        while not self.should_exit_:
            work = None

            with self.work_available_:

                while not self.work_ and not self.should_exit_:
                    self.work_available_.wait()

                if self.work_ and not self.should_exit_:
                    work = self.work_.pop()

            if work:
                try:
                    work.callback()

                except plumbum.commands.ProcessExecutionError:
                    sys.exit( -1 )

                with self.work_complete_:
                    self.work_complete_.notify()
