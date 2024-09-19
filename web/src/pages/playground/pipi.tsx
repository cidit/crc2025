import { useState } from "react"

export default function PIPI() {

    const [color, setcolor] = useState("bg-red-300")

    function onClickHandler() {
        if (color === "bg-blue-300") {
            setcolor("bg-red-300")
        } else {
            setcolor("bg-blue-300")
        }
    }

    return (
        <div>SALUT GROS CON

            <button className={color}
                onClick={onClickHandler}
            >change couleur</button>
        </div>
    )
}