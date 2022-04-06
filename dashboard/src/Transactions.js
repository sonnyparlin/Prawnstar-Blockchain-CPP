import React from 'react';

class Transactions extends React.Component {

    constructor(props) { 
        super(props);
        this.state = { transactions: [] };
    }

    componentDidMount() {
        this.getWalletAmount();
    }

    getWalletAmount() {

        fetch("http://127.0.0.1:8001/nodetransactions")
            .then(response => response.json())
            .then((jsonData) => {
                // jsonData is parsed json object received from url
                this.state.transactions.push( jsonData );
                this.setState({transactions: this.state.transactions[0]})
            })
            .catch((error) => {
                // handle your errors here
                console.error(error);
            });
    }

    render() {         
        return (
            <small>
                <ul className="txUL">
                    {this.state.transactions.map(item => 
                        <li className="noLI" key={item}>
                            <a href={"http://127.0.0.1:8001/tx/" +item}>{item}</a>
                        </li>)
                    }
                </ul>
            </small>
        );
    }
}

export default Transactions;