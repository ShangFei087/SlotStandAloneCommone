// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Item {
    id:mpcButton

    signal buttonClicked

    property string bkColor:"black"
    property string textColor:"white"

    property string borderColor:"white"
    property int    borderWidth:2

    property string btnText:"button"


    Rectangle{
        id: rectBtn
        width: parent.width
        height: parent.height
        radius: 5
        color: bkColor
        border.width: borderWidth
        border.color: borderColor
        opacity: 1.0
        state:parent.state

        Text{
            width: parent.width
            height: parent.height
            text:btnText
            color: textColor

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        MouseArea
        {
            width: parent.width
            height: parent.height
            anchors.fill: parent

            hoverEnabled: true

            onHoveredChanged: {
                if(containsMouse===true)
                {
                    rectBtn.opacity=1.0
                    animateOpacity.pause();
                }
                else
                {
                    animateOpacity.resume();
                }


            }

            onClicked:
            {
                //animateOpacity.stop();
                rectBtn.opacity=1.0
                buttonClicked();
            }
        }

        SequentialAnimation
        {
            id:animateOpacity
            running: true
            loops: Animation.Infinite


            NumberAnimation {
                    id: animateOpacityInvisible
                    target: rectBtn
                    properties: "opacity"
                    from: 1.0
                    to: 0

                    duration: 2000

                    //easing {type: Easing.OutBack; overshoot: 4000}

               }

            NumberAnimation {
                    id: animateOpacityVisible
                    target: rectBtn
                    properties: "opacity"
                    from: 0
                    to: 1.0
                    duration: 1500

                    //easing {type: Easing.OutBack; overshoot: 4000}

               }
        }

        states: [

            State{

                name:"anim_start"

                 PropertyChanges { target:animateOpacity;running:true}
            },

            State{

                name:"anim_stop"

                PropertyChanges { target:animateOpacity;running:false}
            }

        ]
    }



}
