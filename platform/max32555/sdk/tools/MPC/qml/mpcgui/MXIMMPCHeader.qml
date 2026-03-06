// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Item {

    id: mpcProdArrayHeader

    property int nbProdSubColumn:5
    property int colSpace:5
    property int rowSpace:5

    property int rowHeight:100
    property int globalHeight:rowHeight+colSpace

    property int commDeviceWidth:150
    property int commDeviceHeight:rowHeight

    property int prodColWidth:(parent.width-commDeviceWidth)-(2*rowSpace)
    property int prodColHeight:rowHeight

    property int prodSubColWidth:(prodColWidth-((nbProdSubColumn-1)*rowSpace))/nbProdSubColumn
    property int prodSubColHeight:rowHeight/2

    property int prodProcessWidth:prodSubColWidth
    property int prodProcessHeight:prodSubColHeight

    property int prodPhaseWidth:prodSubColWidth
    property int prodPhaseHeight:prodSubColHeight

    property int prodUSNWidth:prodSubColWidth
    property int prodUSNHeight:prodSubColHeight

    property int prodProgressWidth:prodSubColWidth
    property int prodProgressHeight:prodSubColHeight

    property int prodResultWidth:prodSubColWidth
    property int prodResultHeight:prodSubColHeight









    Row
    {
        id:mpcGUIArrayTitles
        spacing:colSpace

        //x:parent.x
        //y:parent.y

        width:parent.width
        height: globalHeight



        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // COMMUNICATION DEVICE ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Rectangle{
                id: mpcGUIArrayTitleCommDevice

                width:commDeviceWidth
                height:rowHeight

                border.width: 2
                border.color: "Black"
                color: "lightgray"
                radius: 5

                Text {
                    id: colNameCommDevice
                    width: parent.width-colSpace
                    height: parent.height-rowSpace
                    text: qsTr("Communication device")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                }


                MouseArea{
                    anchors.fill: parent

                    onClicked: {
                        console.log("click on Commdevice width:"+commDeviceWidth)
                    }
                }

        }



        Column
        {
            id: columnProduction
            spacing:5

            width: prodColWidth
            height: prodColHeight


            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // PRODUCTION COLUMN //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


            Rectangle{
                id: mpcGUIArrayTitleProduction
                y:parent.y+rowSpace

                width:prodColWidth
                height: prodSubColHeight

                border.width: 2
                border.color: "Black"
                color: "lightgray"
                radius: 5

                Text {
                    id: colNameProduction
                    width: parent.width-colSpace
                    height: parent.height-rowSpace
                    text: qsTr("Production")
                    verticalAlignment: Text.AlignTop
                    horizontalAlignment: Text.AlignHCenter

                }

                MouseArea{
                    anchors.fill: parent

                    onClicked: {
                        console.log("click on MPCGUIArrayTitleProduction width:"+prodColWidth)//MPCGUIArrayTitleProduction.width)
                        console.log("click on MPCGUIArrayTitleProduction header:"+prodSubColHeight)
                    }
                }
            }

            Row
            {
                id:rowProdSubTitles
                spacing:colSpace

                width:parent.width
                height: prodSubColHeight


                ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // PRODUCTION SUB-COLUMNS /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //-- Columns number = 5: "Process","Phase", "USN", "Progress", "Result"
                //-- Their width are equal.
                Rectangle{
                    id: mpcGUIArrayTitleProdProcess


                    width:prodSubColWidth
                    height: parent.height-colSpace

                    border.width: 1
                    border.color: "Black"
                    color: "gray"
                    radius: 5

                    Text {
                        id: colNameProductionProcess
                        width: parent.width-colSpace
                        height: parent.height-rowSpace
                        text: qsTr("Process")
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                    }

                }

                Rectangle{
                    id: mpcGUIArrayTitleProdPhase

                    width: prodSubColWidth
                    height: parent.height-colSpace

                    border.width: 1
                    border.color: "Black"
                    color: "gray"
                    radius: 5

                    Text {
                        id: colNameProductionPhase
                        width: parent.width-5
                        height: parent.height-rowSpace
                        text: qsTr("Phase")
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                    }

                }

                Rectangle{
                    id: mpcGUIArrayTitleProdUSN

                    width: prodSubColWidth
                    height: parent.height-colSpace

                    border.width: 1
                    border.color: "Black"
                    color: "gray"
                    radius: 5

                    Text {
                        id: colNameProductionUSN
                        width: parent.width-5
                        height: parent.height-rowSpace
                        text: qsTr("USN")
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                    }

                }

                Rectangle{
                    id: mpcGUIArrayTitleProdProgress

                    width: prodSubColWidth
                    height: parent.height-colSpace

                    border.width: 1
                    border.color: "Black"
                    color: "gray"
                    radius: 5

                    Text {
                        id: colNameProductionProgress
                        width: parent.width-5
                        height: parent.height-5
                        text: qsTr("Progress")
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                    }

                }


                Rectangle{
                    id: mpcGUIArrayTitleProdResult

                    width:prodSubColWidth
                    height:parent.height-colSpace

                    border.width: 1
                    border.color: "Black"
                    color: "gray"
                    radius: 5

                    Text {
                        id: colNameProductionResult
                        width: parent.width-5
                        height: parent.height-5
                        text: qsTr("Result")
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter

                    }

                }


            }


        }


    }






}

