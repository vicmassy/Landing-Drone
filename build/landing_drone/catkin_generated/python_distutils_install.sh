#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
    DESTDIR_ARG="--root=$DESTDIR"
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/victor/catkin_ws/src/landing_drone"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/victor/catkin_ws/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/victor/catkin_ws/install/lib/python2.7/dist-packages:/home/victor/catkin_ws/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/victor/catkin_ws/build" \
    "/usr/bin/python" \
    "/home/victor/catkin_ws/src/landing_drone/setup.py" \
    build --build-base "/home/victor/catkin_ws/build/landing_drone" \
    install \
    $DESTDIR_ARG \
    --install-layout=deb --prefix="/home/victor/catkin_ws/install" --install-scripts="/home/victor/catkin_ws/install/bin"
