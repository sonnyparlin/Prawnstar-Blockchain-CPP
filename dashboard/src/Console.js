import React, {useState, useEffect} from 'react';

export default function App() {

    const [console, setConsole] = useState();

    useEffect(() => {
        const getNodeID = () => {

            fetch("http://127.0.0.1:8001/console")
                .then(response => response.json())
                .then((jsonData) => {
                    // jsonData is parsed json object received from url
                    setConsole(jsonData.console);
                    // console.log(this.state.console);
                })
                .catch((error) => {
                    // handle your errors here
                    console.error(error);
                });
        }

        
        getNodeID();
    });

    return (
        <div className="console">
            <pre>
                <small>{console}</small>
            </pre>
        </div>
    );
}