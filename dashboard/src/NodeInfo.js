import './App.css';
import React from 'react';

class NodeInfo extends React.Component {

    constructor(props) { 
        super(props);
        this.state = { nodeid: "", ip: "", port: "", master: "" };
    }

    componentDidMount() {
        this.getNodeID();
    }

    getNodeID() {

        fetch("http://127.0.0.1:8001/node")
            .then(response => response.json())
            .then((jsonData) => {
                // jsonData is parsed json object received from url
                this.setState({ nodeid: jsonData.message });
                this.setState({ ip: jsonData.ip });
                this.setState({ port: jsonData.port });
                this.setState({ master: jsonData.master });
            })
            .catch((error) => {
                // handle your errors here
                console.error(error);
            });
    }

    render() { 
        return (
            <div>
            <b>Node: {this.state.nodeid}</b><br/>
            Node IP: {this.state.ip}<br/>
            Node Port: {this.state.port}<br/>
            Master Node: {this.state.master}<br/><br/>
            <button className="btn btn-primary">Configure Node</button>
            </div>
        );
    }
}

export default NodeInfo;