import React from 'react';

class App extends React.Component {

    constructor(props) { 
        super(props);
        this.state = { console: "" };
    }

    componentDidMount() {
        this.getNodeID();
    }

    getNodeID() {

        fetch("http://127.0.0.1:8001/console")
            .then(response => response.json())
            .then((jsonData) => {
                // jsonData is parsed json object received from url
                this.setState({ console: jsonData.console });
                // console.log(this.state.console);
            })
            .catch((error) => {
                // handle your errors here
                console.error(error);
            });
    }

    render() { 
        return (
            <div className="console">
                <pre>
                    <small>{this.state.console}</small>
                </pre>
            </div>
        );
    }
}

export default App;
