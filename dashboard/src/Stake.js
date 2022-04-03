import React from 'react';

class Stake extends React.Component {

    constructor(props) { 
        super(props);
        this.state = { stake: "" };
    }

    componentDidMount() {
        this.getWalletAmount();
    }

    getWalletAmount() {

        fetch("http://127.0.0.1:8001/nodewallet")
            .then(response => response.json())
            .then((jsonData) => {
                // jsonData is parsed json object received from url
                this.setState({ stake: jsonData.nodestake });
            })
            .catch((error) => {
                // handle your errors here
                console.error(error);
            });
    }

    render() { 
        return (
            <pre>
                <small>Stake: {this.state.stake} Staked Prawn</small>
                <br/>
            </pre>
        );
    }
}

export default Stake;